#!/usr/bin/python3
import numpy as np
import random as rnd
from multiprocessing import shared_memory
import struct
import sys
import asyncio
import atexit
import time

def create_buffer(name, size):
    shm = shared_memory.SharedMemory(name=name, create=True, size=size)
    b = struct.pack('iii', 2, 0, 0)
    shm.buf[:12] = bytearray(b)
    return shm

def clear_buffer(view, shm):
    view.release()
    shm.close()
    shm.unlink()

def rand_init(matrix):
    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[1]):
            matrix[i, j] = rnd.randint(0, 1000)

def one_init(matrix):
    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[0]):
            matrix[i, j] = 1

def calc_correct(matrix, power):
    if matrix.shape[0] > 1000:
        size = matrix.shape[0]
        return np.array([[pow(size, power - 1, 2 ** 32) for j in range(size)] for i in range(size)], dtype=np.uint32)
    return np.linalg.matrix_power(matrix, power)

async def main():
    buffer_size = 64 * 1024 * 1024
    shm_name = "test_shm_{}".format(rnd.randint(0, 100))
    shm = create_buffer(shm_name, buffer_size)
    data_view = memoryview(shm.buf)
    rnd.seed(36475)
    np.random.seed(374754)

    atexit.register(lambda: clear_buffer(data_view, shm))

    args = [shm_name] + sys.argv[2:]
    process = await asyncio.create_subprocess_exec(sys.argv[1], *args)

    inputs = [
        ((2, 10), 17239),
        ((50, 300), 7689),
        ((500, 700), 435),
        ((2000, 2100), 8)
    ]
    for (l_size, r_size), power in inputs:
        size = rnd.randint(l_size, r_size)
        print('Running test with size={} and pow={}'.format(size, power), file=sys.stderr)
        b = struct.pack('ii', power, size)
        shm.buf[4:12] = bytearray(b)
        byte_count = 4 * size * size
        matrix = np.ndarray((size, size), dtype=np.uint32, buffer=data_view[12:12 + byte_count], order='C')
        if size > 1000:
            one_init(matrix)
        else:
            rand_init(matrix)
        correct_start_ts = time.time()
        correct = calc_correct(matrix, power)
        correct_finish_ts = time.time()

        shm.buf[:4] = bytearray(struct.pack('i', 0))
        result_start_ts = time.time()
        while True:
            cur_status = struct.unpack('i', data_view[:4])
            if cur_status[0] == 2:
                break
        result_finish_ts = time.time()
        
        result = np.frombuffer(data_view[12+byte_count:], dtype=np.uint32, count=size * size).reshape((size, size))
        if not (correct == result).all():
            print('Incorrect result', file=sys.stderr)
            process.kill()
            sys.exit(1)
        print('correct result took', correct_finish_ts - correct_start_ts)
        print('result took', result_finish_ts - result_start_ts)

    shm.buf[:4] = bytearray(struct.pack('i', -1))
    print('Waiting for stop', file=sys.stderr)
    await process.wait()
    assert process.returncode == 0

if __name__ == '__main__':
    asyncio.run(main())
