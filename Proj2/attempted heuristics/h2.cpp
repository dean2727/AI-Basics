// 2
int State::heuristic(State* goal) {
    int h = 0;

    vector<vector<char> > goalConfig = goal -> getConfig();
    int thisBlockConfigSize = blockConfig.size(), goalBlockConfigSize = goalConfig.size();
    int thisStackSize, goalStackSize;

    int numToMove;
    int x0, y0, x, y;
    char thisBlock;

    /* since we have at most 10 stacks and 20 blocks, we can afford an algorithm with nested for loops
    for each block in this state, loop through the vectors of the goal state, incrementing/decrementing x and y coordinates
    then, take the square root of the squared distances of x and y */
    for (int tbcs = 0; tbcs < thisBlockConfigSize; tbcs++) {  // tbcs = this block config stack
        x0 = tbcs;

        thisStackSize = blockConfig[tbcs].size();
        for (int tb = 0; tb < thisStackSize; tb++) {  // tb = this block

            numToMove = 0;
            y0 = tb;
            thisBlock = blockConfig[x0][y0];
 
            for (int gbcs = 0; gbcs < goalBlockConfigSize; gbcs++) {  // gcbs = goal block config stack
                x = gbcs;

                goalStackSize = goalConfig[gbcs].size();
                for (int gb = 0; gb < goalStackSize; gb++) {  // gb = goal block

                    y = gb;

                    // if goal block is equal to this block, get the difference squared, add to sum
                    if (goalConfig[x][y] == thisBlock) {
                        numToMove += abs(x0 - x) + abs(y0 - y);
                        //cout << "For block " << thisBlock << ", we have " << numToMove << endl;
                    }
                }
            }

            h += numToMove;
        }
    }

    return h;
}
