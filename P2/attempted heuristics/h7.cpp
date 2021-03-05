// 8. 2x the number of blocks needing to be moved once, which are blocks that sit on blocks different than 
// what they sit on in the goal state (or has such a block below it) and 4x the number of blocks needing
// to be moved twice, which are blocks that have the correct block below it but need to be moved or have moved-twice blocks
// somewhere below it
int State::heuristic(State* goal) {
    int h = 0;

    vector<vector<char> > goalConfig = goal -> getConfig();
    int thisBlockConfigSize = blockConfig.size(), goalBlockConfigSize = goalConfig.size();
    int thisStackSize, goalStackSize;
    int moveOnce = 0, moveTwice = 0;
    
    //cout << "DEBUG: getting heuristic. this state is\n";
    //printHorizontal();

    // maps each block in the goal state to the block that should be under it
    map<char, char> underBlocks;
    // maps each block to the stack they are on
    map<char, int> stackNums;
    for (int gbcs = 0; gbcs < goalBlockConfigSize; gbcs++) {  // gcbs = goal block config stack
        goalStackSize = goalConfig[gbcs].size();
        for (int gb = 0; gb < goalStackSize; gb++) {  // gb = goal block
            stackNums.insert(make_pair(goalConfig[gbcs][gb], gbcs));

            // no block under it, just have a placeholder of !
            if (gb == 0) {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], '!'));
            }
            else {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], goalConfig[gbcs][gb-1]));
            }
        }
    }

    char thisBlock, blockUnder;

    for (int tbcs = 0; tbcs < thisBlockConfigSize; tbcs++) {  // tbcs = this block config stack
        thisStackSize = blockConfig[tbcs].size();
        bool badUnder = false, badStack = false;
        for (int tb = 0; tb < thisStackSize; tb++) {  // tb = this block
            thisBlock = blockConfig[tbcs][tb];

            // just need to move the block once if the block isnt in the right stack
            // if (stackNums[thisBlock] != tbcs) {
            //     moveOnce++;
            //     badStack = true;
            // }

            // // else, get the block below it
            // else {
            //     if (tb == 0) blockUnder = '!';
            //     else blockUnder = blockConfig[tbcs][tb - 1];

            //     // if correct stack but if a block to move once was found under it, move twice
            //     if (blockUnder != '!' && stackNums[thisBlock] == tbcs && badStack) {
            //         moveTwice++;
            //     }
            // }

            // if block under is not equal
            if (blockUnder != underBlocks[thisBlock]) {
                // checking for move-twice blocks first
                //if (badStack) moveTwice++;
                // else, increment number of blocks to move once
                //else {
                    moveOnce++;
                    badUnder = true;
                //}
            }

            else if (badUnder) moveOnce++;

            // else, if theres no block under, and if in the wrong stack, its a block to move once
            // else if (blockUnder == '!') {
            //     if (stackNums[thisBlock] != tbcs) moveOnce++;
            // }

            // else, if block is under and this block matches the under block in the goal state
            else {
                // if we need to move the block under to a different stack, make this a move twice block
                if (tbcs != stackNums[thisBlock]) {
                    moveTwice++;
                    badStack = true;
                }

                // else, if there is a move twice block under this block, incr moveTwice
                else if (badStack) {
                    moveTwice++;
                }

                // else, if there is a move once block under this block, incr moveOnce
                // if (badUnder) {
                //     moveOnce++;
                // }
            }
        }
    }

    h = 2 * moveOnce + 4 * moveTwice;
    //cout << "DEBUG: got " << h << endl;
    return h;
}