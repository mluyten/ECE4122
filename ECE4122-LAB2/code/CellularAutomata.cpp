#include "CellularAutomata.h"

void ageCell(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    int sum = 0;
    for (int i(r > 0 ? -1 : 0); i <= int(r < thisGeneration.rows()-1 ? 1 : 0); i++) {
        for (int j(c > 0 ? -1 : 0); j <= int(c < thisGeneration.cols()-1 ? 1 : 0); j++) {
            if (j != 0 || i != 0)
                sum += lastGeneration[r+i][c+j];
        }
    }

    if (lastGeneration[r][c] && (sum < 2 || sum > 3))
        thisGeneration[r][c] = 0;
    else if (!lastGeneration[r][c] && sum == 3)
        thisGeneration[r][c] = 1;
    else
        thisGeneration[r][c] = lastGeneration[r][c];
    return;
}

void ageRow(size_t r, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, std::atomic_bool* done) {
    for (size_t c = 0; c < thisGeneration.cols(); c++) {
        ageCell(r, c, thisGeneration, lastGeneration);
    }
    *done = true;
}

int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    double tick = omp_get_wtime();
    for (size_t r = 0; r < thisGeneration.rows(); r++) {
        for (size_t c = 0; c < thisGeneration.cols(); c++) {
            int sum = 0;
            for (int i(r > 0 ? -1 : 0); i <= int(r < thisGeneration.rows()-1 ? 1 : 0); i++) {
                for (int j(c > 0 ? -1 : 0); j <= int(c < thisGeneration.cols()-1 ? 1 : 0); j++) {
                    if (j != 0 || i != 0)
                        sum += lastGeneration[r+i][c+j];
                }
            }

            if (lastGeneration[r][c] && (sum < 2 || sum > 3))
                thisGeneration[r][c] = 0;
            else if (!lastGeneration[r][c] && sum == 3)
                thisGeneration[r][c] = 1;
            else
                thisGeneration[r][c] = lastGeneration[r][c];
        }
    }
    lastGeneration = thisGeneration;
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}

int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    double tick = omp_get_wtime();
    std::vector<Worker> threads(nThreads);
    size_t row = 0;
    while (true) {
        bool allDone = true;
        for (size_t j = 0; j < threads.size(); j++) {
            if (threads[j].idle && row < thisGeneration.rows()) {
                threads[j].idle = false;
                threads[j].done = false;
                threads[j].thread = new std::thread(ageRow, row, std::ref(thisGeneration), std::ref(lastGeneration), &threads[j].done);
                threads[j].thread->detach();
                row++;
            }
            else {
                if (threads[j].done && !threads[j].idle) {
                    delete threads[j].thread;
                    threads[j].idle = true;
                }
            }
            allDone = allDone && threads[j].idle;
        }

        if (row == thisGeneration.rows() && allDone)
            break;
    }
    lastGeneration = thisGeneration;
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}

int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    double tick = omp_get_wtime();
    #pragma omp parallel for  collapse(2) num_threads(nThreads)
    for (size_t r = 0; r < thisGeneration.rows(); r++) {
        for (size_t c = 0; c < thisGeneration.cols(); c++) {
            ageCell(r, c, thisGeneration, lastGeneration);
        }
    }
    lastGeneration = thisGeneration;
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}
