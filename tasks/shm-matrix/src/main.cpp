#include <stdexcept>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>
#include <unistd.h>
#include <atomic>
#include "matutil.h"

#define DATA_READY 0
#define CALC_STARTED 1
#define CALC_FINISHED 2
#define STOP_AND_EXIT -1


struct Header {
    std::atomic<int32_t> flag;
    int32_t n;
    int32_t m;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: [shared memory region name]";
        return 1;
    }

    const char* shm_name = argv[1];

    int shm_fd = shm_open(shm_name, O_RDWR, 0);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    struct stat sb;
    if (fstat(shm_fd, &sb) == -1) {
        perror("fstat failed");
        close(shm_fd);
        return 1;
    }
    size_t SIZE = sb.st_size;

    void* mem = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return 1;
    }

    auto* header = reinterpret_cast<Header*>(mem);

    while (true) {
        int32_t st;
        do {
            st = header->flag.load();
        } while (st != DATA_READY && st != STOP_AND_EXIT);

        if (st == STOP_AND_EXIT) {
            break;
        }

        header->flag.store(CALC_STARTED);

        int32_t n = header->n;  
        int32_t m = header->m; 

        uint8_t* raw_matrix = static_cast<uint8_t*>(mem) + sizeof(Header);
        const uint32_t* matrix_data = reinterpret_cast<const uint32_t*>(raw_matrix);
        uint32_t* result = reinterpret_cast<uint32_t*>(raw_matrix + sizeof(uint32_t)*m*m);

        mat_pow(matrix_data, result, m, n);

        header->flag.store(CALC_FINISHED);
    }

    munmap(mem, SIZE);
    close(shm_fd);
}
