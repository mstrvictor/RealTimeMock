# Real time Mock Trading system implementation.

## Taking liberal inspiration from:
  1. SQT Mock Trading
  2. Optiver Delta Dash

## Game sequence:
    1. initialisation
        a. Host sets questions and answers (initialise_question)
        b. Lobby Starts (dw for offline implementation)
        c. Players connect (dw for offline implementation)
    2. Gameloop
        a. Question is shown to all players 
        b. Players can choose to make bid/ask offers
        c. Bid/Ask offers are recorded into orderbook (Pointers at highest bid
        vs lowest ask)
        d. matched trades are notified, unmatched remain until canceled
        e. new information is dropped, loop to b, else progress
    3. Settlement
        a. hosts answer is used to settle market.
        b. scores/current positions are settled.
        c. Players are ranked based on their final position and score.
        
    4. Looping between 2. and 3.
    
    5. Final
        a. Players are scored based off their peformance for every single round
        either:
            A) Players are scored based on standard distribution of scores
            B) Players are scored off position
            C) Players are scored off Money made
        b. Players are ranked accordingly. 
  
## Discussion
    1. Datastructure of orderbook?
    2. Main function points to "mini-main" functions, such as main for seq 1,
    seq 2 etc. Make a graph/network to make everything more interpretable. 
    3. Running multiple games simultaneously? Honestly thats going to be a hard
    ask, probably only 1 game at a time. Give someone backdoor handling in the 
    server, maybe operate everything via terminal. Or just make an admin panel
    UNSW quantsoc email logins to/controls. 
    4. The more I work with the project the better I understand web dev. This is
    the easy part lol
