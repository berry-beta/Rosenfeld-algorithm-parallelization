# Rosenfeld-algorithm-parallelization
School project for which we were asked to parallelize the Rosenfled algorithm, a connected component labelling algorithm (https://en.wikipedia.org/wiki/Connected-component_labeling). We were given the implantation of the algorithm, my code is in the LabelRosenfeld.cpp file starting line 310. 

The challenges for parallelizing this algorithm are:
1. The label of each pixel depends of the label of previous pixel
2. The algorithm loop through the image in a sequential order

Therefore parallelization by only slicing the image will result in objects "cut" where slices meet. We developped the following algorithm to mitigate this issue:

1. Slicing the image (1 slide per thread) and applying the algorithm on each. This is the part that is paralellized 
2. Identification of equivalences at the border of each slice
3. Setting up and solving a global equivalence table
4. Updating the final image


