#include "pyvis/PythonVisualizer.h"
#include "writer.h"
#include <vector>
#include <iostream>

int size;//size of the grid
//make configurabel by argv need to convert argv to int and assign 
//in main, also get rid of constexpr
//need more of the test outputs to compare
//checks for a critical position in the sandpile
bool critical(std::vector<std::vector<int>> pile){
    bool isCritical = false;
    //run through the sandpile
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            if (pile[i][j] >= 4)//if the current position is critical
            {
                isCritical = true;
            }
        }
    }
    return isCritical;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "You must enter a size of grid to use!" << std::endl;
        return 1;
    }
    size = std::stoi(argv[1]);
    using Sandpile = std::vector<std::vector<int>>;
    using SandpileStates = std::vector<Sandpile>;
    
    SandpileStates data;
    Sandpile sand(size, std::vector<int>(size));//initialize sand to the size of the grid

    // ... initialize sand ...
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            if (i == j){
                sand[i][j] = 10;
            }else 
            {
                sand[i][j] = 5;
            }
        }
    }
    //push back initial state
    data.push_back(sand);

    while(critical(sand)){//as long as there is a critical cell
        //go through the sandpile
        for (int i = 0; i < size; ++i){
            for (int j = 0; j < size; ++j){
                if (sand[i][j] >= 4){//if cell is critical
                    sand[i][j] -= 4;
                    
                    //j is colomns
                    //i is rows
                    // up: sand[i-1][j] += 1;
                    // down: sand[i+1][j] += 1;
                    // left: sand[i][j-1] += 1;
                    // right: sand[i][j+1] += 1;
                    
                    //if on left side
                    if (j == 0){
                        if (i == 0){
                            //top left : add to the right and below
                            sand[i][j+1] += 1; //right
                            sand[i+1][j] += 1; //down
                        }else if (i > 0 && i < (size-1)){
                            //between top and bottom on left
                            //add up, down, right
                            sand[i-1][j] += 1; // up
                            sand[i+1][j] += 1; //down
                            sand[i][j+1] += 1; //right
                        }else if (i == (size-1)){
                            //bottom left: add up, right
                            sand[i-1][j] += 1; // up
                            sand[i][j+1] += 1; //right
                        }
                    }else if (j == (size-1)){//if on right side
                        if (i == 0){
                            //topRigth: add left, down
                            sand[i+1][j] += 1; //down
                            sand[i][j-1] += 1; //left
                        }else if (i > 0 && i < (size-1)){
                            //between top and bottom on right
                            //add up,down,left
                            sand[i-1][j] += 1; // up
                            sand[i+1][j] += 1; //down
                            sand[i][j-1] += 1; //left
                        }else if (i == (size-1)){
                            //bottom right: add up and left
                            sand[i-1][j] += 1; // up
                            sand[i][j-1] += 1; //left
                        }
                    }else //between left and right side
                    {
                        if (i == 0){
                            //top: add left,right down
                            sand[i+1][j] += 1; //down
                            sand[i][j-1] += 1; //left
                            sand[i][j+1] += 1; //right
                        }else if (i > 0 && i < (size-1)){
                            //between top and bottom
                            //add up,down,left,right
                            sand[i-1][j] += 1; // up
                            sand[i+1][j] += 1; //down
                            sand[i][j-1] += 1; //left
                            sand[i][j+1] += 1; //right
                        }else if (i == (size-1)){
                            //bottom: add left,right,up
                            sand[i-1][j] += 1; // up
                            sand[i][j-1] += 1; //left
                            sand[i][j+1] += 1; //right
                        }
                    }
                }
            }
        }
        //push back the state after every run through the sandpile
        data.push_back(sand);
    }
    write_data(sand);
    //make the python visualizer to display the bokeh plot
    py::PythonVisualizer pyvis({"../.."});
    auto figure = pyvis.make_new_figure("Abelian Sandpile Model", py::kwarg("x_range", std::vector<int>{0,size}),  py::kwarg("y_range", std::vector<int>{0,size}));
    auto ticker = pyvis.ticker("BasicTicker");
    auto color_mapper = pyvis.color_mapper("LinearColorMapper", py::kwarg("palette", "Spectral11"), py::kwarg("low", 0), py::kwarg("high", 10));
    pyvis.color_bar(figure, py::kwarg("color_mapper", color_mapper), py::kwarg("ticker", ticker), py::kwarg("location", std::vector<int>{0,0}));
    auto im = pyvis.image(figure, data[0], py::kwarg("color_mapper", color_mapper));
    auto slider = pyvis.slider(im, "Frame", 0, data.size()-1, py::kwarg("image", data));
    auto layout = pyvis.layout(figure, slider);
    pyvis.generate_html(layout, "sandpile.html");
}