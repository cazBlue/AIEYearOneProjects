function GetLocalScore()
{   //returns the local score        
  if(typeof(Storage) !== "undefined") //check if supported
        if(localStorage.getItem("Match3Score") !== null)
            return parseInt(localStorage.getItem("Match3Score"));
        else
            return 0;
  else
  {
      console.log("local high scores not supported");
      return 0;          
  }
};    

function SetLocalScore(score)
{
   //sets the local score to the current score
  if(typeof(Storage) !== "undefined") //check if supported
      localStorage.setItem("Match3Score", score);
  else
      console.log("local high scores not supported");              
};

function grid(a_marginLeft, a_marginTop, a_gridXcount, a_gridYcount, a_cellSize)
{
    //  ------------    grid size/position config
    this.marginLeft = a_marginLeft;
    this.marginTop = a_marginTop;
    this.gridXCount = a_gridXcount;
    this.gridYCount = a_gridYcount;
    this.cellSize = a_cellSize;
    //  ---------------- 

    this.activePiece = undefined; //ptr to active piece, allows blocks to recieve player input
    this.grid = []; //create grid array
    this.heavy = []; //list of "heavy" blocks, these are blocks that have a piece missing underneath
    this.toProcess = [];
    this.dirty = [];
    
    //SetLocalScore(0); //reset the local storage score    
    this.score = GetLocalScore();    
    
    //Creates a web worker which tracks current amount of each colour block on the grid
    this.webWorker = new WebWorker(this);
    this.webWorker.init();        
    
    this.scoreText = game.add.text(10, 10, "score : 0",{
            font: "20px Arial",
            fill: "#d4d4d4",
            align: "left"                    
        });      
    
    this.creditText = game.add.text(game.width - 200, game.height - 80, "Created By:\n\
Callan Winfield\n\
2015 - www.callanw.com\n\
",{
        font: "14px Arial",
        fill: "#d4d4d4",
        align: "left"                    
    });                
    
    this.gridBG = game.add.graphics(0, 0); //init in create function
    //draw the background grid colour
    this.gridBG.beginFill(535353, 1) ;
    this.gridBG.drawRect(this.marginLeft, this.marginTop, 
        (this.gridXCount * this.cellSize), (this.gridYCount * this.cellSize));    
    
    //  Register the keys.
    this.leftKey    = game.input.keyboard.addKey(Phaser.Keyboard.LEFT);
    this.rightKey   = game.input.keyboard.addKey(Phaser.Keyboard.RIGHT);
    this.spaceKey   = game.input.keyboard.addKey(Phaser.Keyboard.SPACEBAR);
       
    //  Stop the following keys from propagating up to the browser
    game.input.keyboard.addKeyCapture([ Phaser.Keyboard.LEFT, Phaser.Keyboard.RIGHT, Phaser.Keyboard.SPACEBAR ]);    
    
    this.Create = function()
    {
        //set the grid array up, grid is defined by y columns
        for(var i = 0; i < this.gridXCount; ++i)        
            this.grid[i] = new Array();        

        //populate the grid array
        var arrayDebugDisplay = "";
        for(var y = 0; y < this.gridYCount; ++y )
        {
            for(var x = 0; x < this.gridXCount; ++x )
            {            
                this.grid[x][y] = "null";

                arrayDebugDisplay += x + "," + y + " : ";
            }       
            arrayDebugDisplay += "\n";
        }
        console.log("grid array layout \n" + arrayDebugDisplay);
        
        //-----------------------------       draw  grid
        this.wall = game.add.graphics(0, 0); //init rect
        this.wall.lineStyle(2, 0xFF0000, 1);    

        for (var i = 0; i <= a_gridXcount; i++)
        {
            //create vertical line
            var xPos = a_marginLeft + (i * this.cellSize); //grab the x pos
            this.wall.moveTo(xPos, this.marginTop);        //start point
            this.wall.lineTo(xPos, this.marginTop + this.gridYCount * this.cellSize); // end poin
        }

        for (var i = 0; i <= a_gridYcount; i++)
        {
            //create horizontal line
            var yPos = this.marginTop + i * this.cellSize;

            this.wall.moveTo(a_marginLeft, yPos);        
            this.wall.lineTo(a_marginLeft + ((this.gridXCount) * this.cellSize), yPos);
        }    
        //--------------------------------------------------------------    
        
        //draw blue helpers for touch
        this.helperLine = game.add.graphics(0, 0); //init rect
        this.helperLine.lineStyle(2, 0x0036ff, 1);            
        
        this.helperLine.moveTo(5, 0);        
        this.helperLine.lineTo(5, 590);        
        
        this.helperLine.moveTo(790, 0);        
        this.helperLine.lineTo(790, 590);                
    };
    
    this.init = function()
    {                
        this.createPeice(); 
    };   
    
    this.PopBlocks = function(blocks)
    {                   
        for(var i = blocks.length - 1; i >= 0; --i) 
        {   //make sure no duplicates
            //var block = this.grid[blocks[i].gridX][blocks[i].gridY];            
            
            var block = blocks[i];
            
            block.sprite.destroy(true); //destroy the matching blocks and remove them            
            block.dirty = true;
            
            this.RemoveFromGrid(block);            
            block = null;            
    
        }   
    };
    
    this.GetMatches = function(block)
    {        
        var match = [];
        if(block.gridY >= 0) //make sure the block is on the grid
        {   
            //make sure to check within the grid
            var right = this.CheckDirection         (1, 0 ,     block.gridX, block.gridY);
            var left = this.CheckDirection          (-1, 0,     block.gridX, block.gridY);  
            var up = this.CheckDirection            (0, -1,     block.gridX, block.gridY);  
            var down = this.CheckDirection          (0, 1,      block.gridX, block.gridY);  
            var diagUpRight = this.CheckDirection   (1, -1,     block.gridX, block.gridY);  
            var diagDownRight = this.CheckDirection (1, 1,      block.gridX, block.gridY);  
            var diagDownLeft = this.CheckDirection  (-1, 1,     block.gridX, block.gridY);  
            var diagUpLeft = this.CheckDirection    (-1, -1,   block.gridX, block.gridY);  
            
            //check if we have a match of 3, clear the matches if not (does not include starting block            
            if(right.length + left.length <= 1) 
            {
                right = [];
                left = [];
            }
            
            if(up.length + down.length <= 1)
            {
                up = [];            
                down = [];
            }
            
            if(diagDownLeft.length + diagUpRight.length <= 1)
            {
                diagDownLeft = [];
                diagUpRight = [];
            }
            
            if(diagDownRight.length + diagUpLeft.length <= 1)
            {
                diagDownRight = [];
                diagUpLeft = [];
            }
            
            match = right.concat(left, up, down, diagDownLeft, diagDownRight, diagUpRight, diagUpLeft);
            
            //add the inital block if we have a match
            if(right.length > 0 || left.length > 0 || up.length > 0 || down.length > 0 
                    || diagDownLeft.length > 0 || diagDownRight.length || 0
                    || diagUpRight.length > 0 || diagUpLeft.length > 0)
                match.push(block);
        }
        
        return match;
    };
    
    this.MoveBlock = function(block)
    {   
        //returns true if piece moved
        var hasMoved = false;
        
        //  -----------sprite bottom Y compared to the cell size - detect when in next cell
        if(block.sprite.y + this.cellSize - block.spriteGridPos >= this.cellSize)
        {
            //check the next space below
            if(!this.CheckSpaceBelow(block.gridX, block.gridY))                            
                block.Lock(this);   //lock the block if we are in the next cell                            
            else
            {              
                //block.sprite.y = Math.floor(block.sprite.y); //manually correct the position, this avoids any displacement if the piece is coming to a res            
                block.sprite.y = block.sprite.y = this.marginTop + (this.cellSize * block.gridY); //manually set position
                block.spriteGridPos = block.sprite.y + this.cellSize; //update the current sprite grid position for future comparison

                if(this.grid[block.gridX][block.gridY] !== undefined) 
                {
                    this.RemoveFromGrid(block);
                    block.gridY ++;           
                    this.PutOnGrid(block);
                }
                else                 
                    block.gridY ++; //update positions for pieces above the grid - mainly used for debugging
                                
                hasMoved = true;
            }
        }
        
        return hasMoved;
    };
    
    //TODO remove duplicate function from piece 
    this.RemoveFromGrid = function(block)
    {
        this.grid[block.gridX][block.gridY] = "null"; //clear the current bot position
    };
    
    this.ShakeLooose = function()
    {        
        //not overly effiecient but catches any glitches or missess due to animation timing
        for(var y = 0; y < this.gridYCount; ++y )
        {
            for(var x = 0; x < this.gridXCount; ++x )
            {      
                var block = this.grid[x][y];  
                
                if(block instanceof Block)
                {                    
                    block.sprite.y = block.sprite.y = this.marginTop + (this.cellSize * block.gridY); //manually set position
                    block.spriteGridPos = block.sprite.y + this.cellSize; //update the current sprite grid position for future comparison                                    
                    
                    //check below the block   
                    if(this.CheckSpaceBelow(block.gridX, block.gridY) && !block.active)
                    {
                        var args = [];
                        args.push(block);
                        this.MakeHeavy(this.GetAllAbove(args));
                        this.MakeHeavy(args);                        
                    }
                    else    //check if there are unfound matches
                    {   
                        var matches = this.GetMatches(block);
                        if(matches.length > 0)
                        {                              
                            matches.push(block);
                            
                            //remove from heavy array
                            for(var i = this.heavy.length - 1; i >= 0; --i)
                            {
                                for(var args = matches.length - 1; args >= 0; --args)
                                {
                                    if(this.heavy[i] === matches[args])
                                    {
                                        this.heavy.splice(i,1);
                                        i--;
                                        args--;                                        
                                    }
                                }
                            }                                                        
                            this.PopBlocks(matches); //destroy blocks from grid
                        }
                    }
                }
            }                 
        }        
    };
    
    this.PutOnGrid = function(block)
    {        
        this.grid[block.gridX][block.gridY] = block; //clear the current position 
        
        this.webWorker.DisplayBlockCount();
    };    
    
    
    this.LockAbove = function(block)
    {
      //locks all blocks above passed block
      var blocks = this.GetAllAbove(block);
      
      for(var i = 0; i < blocks.length; ++i)      
          blocks[i].Lock();                
    };
    
    this.GetAllAbove = function(blocks)
    {
        //return a list of all blocks above a position        
        var blocksAbove = [];        
        
        for(var i = 0; i < blocks.length; ++i)
        {
            var isValid = true;
            var posY = 1;   //start above the current block
            
            while(isValid)
            {                
                if( this.grid[blocks[i].gridX][blocks[i].gridY - posY] instanceof Block)
                {
                    var curBlock = this.grid[blocks[i].gridX][blocks[i].gridY - posY]; //grab a new block from above to play with
                                    
                    blocksAbove.push(curBlock);
                    posY ++;                    
                }
                else
                    isValid = false;
            }
        }
        
        return blocksAbove;
    };

    this.CheckDirection = function(dirX, dirY, indX, indY)
    {
        //assumes the block coming in is valid, checks for matches on given direction
        var match = [];                        
        var pos = new Vector2(dirX + indX, dirY + indY);

        if(pos.y >= 0 
            && pos.x >=0 
            && pos.x < this.gridXCount
            && pos.y < this.gridYCount
            && this.grid[pos.x][pos.y] !== undefined
            && this.grid[pos.x][pos.y] instanceof Block)
        {
            var block = this.grid[pos.x][pos.y];
            var matchType = this.grid[indX][indY].type;
            
            if(block.type === matchType)
            {
                match.push(block); //match found
                
                //check next block
                var isMatch = this.CheckDirection(dirX, dirY, pos.x, pos.y);
                if(isMatch.length > 0)
                    match.push(isMatch[0]); //we only push one block at a time
            }            
        }                

        return match;                
    };
    
    this.Update = function(dt)
    {
        //temp score handler
        
        this.scoreText.text = "score: " + this.score;
        
        //MAIN GAME LOGIC
        if(this.activePiece === -1) //game over
           //console.log("the game is over");
           return; //game over man
        else if(this.activePiece.active) //piece is active
        {
            this.CheckInput(dt);
            this.activePiece.Update(dt);            
        }
        else //processess all matching blocks, no player input
        {
            var blockMatches = [];
            var blocksAbove = [];
            
            if(!this.activePiece.checked) //handle active piece
            {
                var botMatch = this.GetMatches(this.activePiece.bot); 
                var midMatch = this.GetMatches(this.activePiece.mid); 
                var topMatch = this.GetMatches(this.activePiece.top); 
                
                blockMatches = botMatch.concat(midMatch, topMatch);                                
                blockMatches = this.RemoveDuplicates(blockMatches);

                blocksAbove = this.GetAllAbove(blockMatches);
                blocksAbove = this.RemoveDuplicates(blocksAbove);
                blocksAbove = this.RemoveMatches(blocksAbove, blockMatches);                                    
                
                this.activePiece.checked = true;    //the active piece will be destoyed, don't check it again
            }                        
            
            if(this.activePiece.checked         //handle process list when everything has stopped
                    && this.heavy.length === 0 
                    && this.toProcess.length > 0)
            {
                blockMatches = this.ProcessBlocks();                
                
                blocksAbove = this.GetAllAbove(blockMatches);
                blocksAbove = this.RemoveDuplicates(blocksAbove);
                blocksAbove = this.RemoveMatches(blocksAbove, blockMatches);                                                    
            }
            
            //TODO add score based on matches
            this.score += blockMatches.length;
            
            //check if there is a new high score
            if(GetLocalScore() < this.score)
                SetLocalScore(this.score);
                        
            this.PopBlocks(blockMatches); //matches are destroyed at this point (and removed from grid)
            
            this.MakeHeavy(blocksAbove);
            
            this.UpdateHeavy(dt);
            
            this.HeavyToProcess();  //take non active blocks and add them to the process list            
            
            //only create a new piece when everything is processed
            if(this.heavy.length === 0 && this.toProcess.length === 0)                
                this.createPeice(); //Create new piece        
        }
    };    
    
    this.CleanDirty = function()
    {
        //check heavy list for blocks that should be destroyed
        for(var i = this.heavy.length - 1; i >= 0; --i) 
        {   //make sure no duplicates
            var block = this.heavy[i];            
            
            if(block.dirty)
            {    
                block.sprite.destroy(true);
                this.RemoveFromGrid(block);
                block = null;
                this.heavy.splice(i,1);
                i--;                
                //console.log("dirty block found  in heavy and removed");            
            }                            
        } 
    };
    
    this.ReAlign = function()
    {
        //scan through the entire grid and re-align all non active peices
        //created as a debugging tool
        for(var y = 0; y < this.gridYCount; ++y )
        {
            for(var x = 0; x < this.gridXCount; ++x )
            {            
                if(this.grid[x][y] instanceof Block)
                {
                    var block = this.grid[x][y];
                    block.sprite.y =  block.sprite.y = this.marginTop + (this.cellSize * block.gridY);
                    if(!block.sprite.visible)
                        block.sprite.visible = true;
                    if(!block.sprite)
                        block.sprite.alive = true;
                }
            }                   
        }        
        console.log("re-aligned");
    };

    this.RemoveDuplicates = function(list)
    {
        //prunes list of any matches from list 2 and returns the cleaned list
        for(var i = list.length - 1; i >= 0; i--)
        {
            for(var x = list.length - 1; x >= 0; x--)
            {
                if(list[x] === list[i] && i !== x)
                {
                    list.splice(x,1);
                    x--;                    
                }
            }
        }
        return list;
    };
    
    this.RemoveMatches = function(listOne, listTwo)
    {
        //prunes list of any matches from list 2 and returns the cleaned list
        for(var i = listOne.length - 1; i >= 0; i--)
        {                       
            for(var x = listTwo.length - 1; x >= 0; x--)
            {
                if(listTwo[x] === listOne[i])
                {             
                    listOne.splice(i,1);                    
                    i--;
                }
            }
        }
        return listOne;
    };
    
    this.ProcessBlocks = function()
    {
        //process blocks looks for matches and returns them
        //blocks above can then be found
        
        var matches = [];
        
        //get all matches
        for(var i = this.toProcess.length - 1; i >= 0; --i)
        {
            var block = this.toProcess[i];
            
            var blockMatches = this.GetMatches(block);
            
            if(blockMatches.length > 0)
                blockMatches.push(block);
            
            matches = matches.concat(blockMatches);            
        }
        
        this.RemoveDuplicates(matches);
        
        this.toProcess = []; //clear for the next update
        
        return matches;        
    };
    
    this.HeavyToProcess = function()
    {   
        
        this.CleanDirty(); //ensure no dirty blocks make it onto the process list
        
        //pushes inactive blocks onto the process list
        for(var i = this.heavy.length - 1; i >= 0; --i)
        {
            if(!this.heavy[i].active)
            {
                this.toProcess.push(this.heavy[i]);
                this.heavy.splice(i, 1);
                --i;
            }
        }
        
        //this.toProcess = []; //temp clean
    };
    
    this.UpdateHeavy = function(dt)
    {
        for(var i = 0; i < this.heavy.length; ++i)
        {
            //if(this.heavy[i] !== undefined 
            //     && this.heavy[i].sprite !== undefined 
            //     && this.heavy[i].sprite !== null)
                this.heavy[i].UpdateHeavy(dt, this);
            //else
            //{
                //console.log("error found purging block");                                
                //this.heavy.splice(i,1);   
                //i--;
            //}
        }
    };
    
    this.MakeHeavy = function(blocks)
    {
        if(blocks.length > 0)
        {
            this.heavy = blocks.concat(this.heavy);                

            //check for duplicates and remove them
            this.heavy = this.RemoveDuplicates(this.heavy);  

            //build an array to hold the sorted columns temporarily
            var yCols = [];        
            for(var i = 0; i < this.gridXCount; ++i)
            {
                yCols[i] = new Array();
            }

            //sort the heavies by column and activate them
            for(var i = 0; i < this.heavy.length; ++i)
            {
                yCols[this.heavy[i].gridX].push(this.heavy[i]);
                this.heavy[i].active = true;
            }
            
            for(var i = 0; i < yCols.length; ++i)
                if(yCols[i].length > 0)
                    yCols[i] = this.QuickSort(yCols[i]) ;  //sort each colmn

            //clear the heavy list
            this.heavy = [];
            
            //rebuild the heavy list with the sorted columns
            
            for(var i = 0; i < this.gridXCount; ++i)
            {            
                this.heavy = this.heavy.concat(yCols[i]); //add column array to heavy
            }
        }
    };
    
    this.QuickSort = function(array)
    { 
        //reference from http://www.sorting-algorithms.com/quick-sort
        // and http://www.sorting-algorithms.com/static/QuicksortIsOptimal.pdf
        //http://betterexplained.com/articles/sorting-algorithms/
        
        var lesser = [];    
        var greater = [];

        //grab a random pivot from the array    
        var index = Math.floor(Math.random() * (array.length - 1));
        if(index < 0)
            index = 0;
        var pivot = array[index].gridY;    
        var pivotObj = array[index];
        array.splice(index, 1); //and remove the pivot from the array

        //sort based on pivot descending
        for(var value in array)
        {
            if(array[value].gridY >= pivot)
                lesser.push(array[value]);
            else
                greater.push(array[value]);
        }

        //check if we are done, if not recursivly go till we are
        if(lesser.length > 1)
            lesser = this.QuickSort(lesser);

        if(greater.length > 1)
            greater = this.QuickSort(greater);

        return lesser.concat(pivotObj,greater);            
    };
    
    this.CheckInput = function(dt)
    {
        if (game.input.pointer1.isDown)
        {
            var xPos = game.input.pointer1.x;
            //TODO use global width
            if(xPos < 800 / 3)
                this.activePiece.MoveLeft(dt);            
            else if(xPos > 800 - (800 / 3))
                this.activePiece.MoveRight(dt);        
            else
                this.activePiece.RotateOrder(dt);                        
        }

        ///check input and pass to active
        if (this.leftKey.isDown)
            this.activePiece.MoveLeft(dt);

        if (this.rightKey.isDown)
            this.activePiece.MoveRight(dt);        

        if (this.spaceKey.isDown)
            this.activePiece.RotateOrder(dt);            
    };
    
    this.CheckSpaceBelow = function(curX, curY)
    {
        //Check if the space below the current y is open
        var isValid = false;
        var resultNull = this.grid[curX][curY + 1] === "null";
        var resultDefined = this.grid[curX][curY + 1] === undefined;    //accept un defined as we start outside the top of the grid
        var isNotBottom = (curY + 1 <= this.gridYCount -1);
                                
        if(resultDefined || resultNull)
            if(isNotBottom)
                isValid = true;
       
       /* //can cause overlapping, instead check the blocks in the to process list if space below is free
        if(this.grid[curX][curY + 1] instanceof Block && this.grid[curX][curY + 1].active)  //check if there is a block below this and the next and if it's active, we can follow moving blocks!            
        {
        //&&  this.grid[curX][curY + 2] instanceof Block && this.grid[curX][curY + 2].active)
           isValid = true;
        }
        */
        
        
        return isValid;
    };
    
    //NOTE: only pass the bottom piece and check 3 spaces on left
    this.CheckSpaceRightFree = function(bot)
    {
        //Check if the space below the current y is open
        var isValid = false;
        var resultBotNull = this.grid[bot.gridX + 1][bot.gridY + 1]       === "null";      
        var resultMidNull = this.grid[bot.gridX + 1][bot.gridY]   === "null";      
        var resultTopNull = this.grid[bot.gridX + 1][bot.gridY - 1]   === "null";      
        
        if(resultBotNull && resultMidNull && resultTopNull)
          isValid = true;
      
      return isValid;
    };    
    
    //NOTE: only pass the bottom piece and check 3 spaces on left
    this.CheckSpaceLeftFree = function(botX, botY)
    {
        //Check if the space below the current y is open
        var isValid = false;
        var resultBotNull = this.grid[botX - 1][botY + 1]       === "null";      
        var resultMidNull = this.grid[botX - 1][botY    ]   === "null";      
        var resultTopNull = this.grid[botX - 1][botY - 1]   === "null";      
      
        if(resultBotNull && resultMidNull && resultTopNull)
          isValid = true;
      
      return isValid;
    };    
    
    ///creates a peice, note that a new peice must be set to active    
    ///returns created peice
    //TODO account for full row
    //is pos is provided it is considered an override -1 to ignore
    this.createPeice = function()    
    {   
        var isValid = false;        
        var gridPlaceX = 0;
        var piece = undefined;
        
        //check if a valid place exists
        var isFree = false;
        for(var i = 0; i < this.gridXCount; ++i)
        {
            //check the current piece and the one above to ensure nothing is coming down
            //check that there are at least 3 spaces for this piece to fit into
            if(this.grid[i][0] === "null" && this.grid[i][1] === "null" && this.grid[i][2] === "null")        
                isFree = true;                               
        }

        if(isFree)
        {
            //find a starting place that is valid
            while(!isValid)
            {
                gridPlaceX = Math.floor(Math.random() * this.gridXCount);  //x grid starting place
                isValid = this.SpawnOnGrid(gridPlaceX);            
            }

            var posX = (gridPlaceX * this.cellSize) + this.marginLeft;
            piece = new Piece(posX, this.marginTop, gridPlaceX, 0 , this);
            this.activePiece = piece;
            this.activePiece.Init();  

            this.grid[gridPlaceX][0] = piece.bot;   //put the first block on the grid
        }
        else
            this.activePiece = -1;
                        
    };
    
    
    //check if a spawn point is valid
    this.SpawnOnGrid = function(newPos)
    {        
        var isValidMove = true;

        if(this.grid[newPos][0] !== "null")
        {
            isValidMove = false;    //spot is not avaliable
        }        
        
        return isValidMove;
    };
        
    this.PrintGrid = function()
    {
        //populate the grid array
        var arrayDebugDisplay = "";
        for(var y = 0; y < this.gridYCount; ++y )
        {
            for(var x = 0; x < this.gridXCount; ++x )       
            {
                
                if(this.grid[x][y] !== "null" && this.grid[x][y] !== undefined)
                {
                    arrayDebugDisplay += "O : ";        
                }
                else
                    arrayDebugDisplay += "X : ";        
                
            }
            arrayDebugDisplay += "\n";
        }

        console.log("grid array layout showing blocks \n" + arrayDebugDisplay);        
    };
}