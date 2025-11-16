#include <vector>
#include <iostream>

constexpr int NumberOfGrids = 16;
constexpr int GridXDimension = 1024;
constexpr int GridYDimension = 1024;
constexpr int NumberOfSims = 10;

class Life {

public:
    using Grid = std::vector<std::vector<int>>;
private:    
    Grid current;
    Grid future;

public:

    void reset(const Grid& grid) {
        current = future = grid;
    }

    int getPopulationCount() {
        int populationCount = 0;
        for (auto& row: current)
            for (auto& item: row)
                populationCount += item;
        return populationCount;
    }

    void printCurrentGrid() {
        for (auto& row: current) {
            for (auto& item: row)
                item ? std::cout << "x " : std::cout << ". ";
            std::cout << "\n";
        }
        std::cout << "\n";
    }    

    // Simulate the next generation of life
    void simulateNext() {
        //printCurrentGrid();
        int M = current.size();
        int N = current[0].size();
        
        // Loop through every cell
        for(int i = 0; i < M; i++) {
            for(int j = 0; j < N; j++) {
                int aliveNeighbours = 0;      
                // finding the number of neighbours that are alive                  

                int ii = i - 1;
                int jj = j - 1;
                int cell = ii < 0 ? 0 : (jj < 0 ? 0 : current[ii][jj]);
                aliveNeighbours += cell;

                cell = ii < 0 ? 0 : current[ii][j];
                aliveNeighbours += cell;

                jj = j + 1;
                cell = ii < 0 ? 0 : (jj < N ? current[ii][jj] : 0);
                aliveNeighbours += cell;

                jj = j - 1;
                cell = jj < 0 ? 0 : current[i][jj];
                aliveNeighbours += cell;

                aliveNeighbours += current[i][j];

                jj = j + 1;
                cell = jj < N ? current[i][jj] : 0;
                aliveNeighbours += cell;

                ii = i + 1;
                jj = j - 1;
                cell = ii < M ? (jj < 0 ? 0 : current[ii][jj]) : 0;
                aliveNeighbours += cell;

                cell = ii < M ? current[ii][j] : 0;
                aliveNeighbours += cell;

                jj = j + 1;
                cell = ii < M ? (jj < N ? current[ii][jj] : 0) : 0;
                aliveNeighbours += cell;

                // The cell needs to be subtracted from
                // its neighbours as it was counted before
                aliveNeighbours -= current[i][j];

                int nextCell = __builtin_unpredictable(aliveNeighbours == 2) ? current[i][j] : 0;
                future[i][j] = __builtin_unpredictable(aliveNeighbours == 3) ? 1 : nextCell;
            }
        }
        std::swap(current, future);
    }
};

// Init random starting grid of the game
Life::Grid initRandom();
// Simulates N steps of the game for each starting grid 
// and return population count
std::vector<int> solution(const std::vector<Life::Grid>& grids);
