/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var directionEnum = 
{
    LEFT : 0,
    RIGHT : 1,
    NONE : 2,
    ROTATE : 3
};

function Piece(posX, posY, gridX, gridY, grid)
{
    this.grid = grid;            
    this.active = true;  //pieces created active
    this.checked = false; //has this piece been handled in the grid
    
    this.top = null;    
    this.mid = null;    
    this.bot = null;
    
    this.lastMoveTime = 0;   //time since last move
    this.minMoveTime = .08;  //minimum time between piece moves in mili seconds
    this.lastMoveDir = directionEnum.NONE; //last direction moved
  
    this.Init = function()
    {
        //create blocks          
        var topID = Math.floor(Math.random() * 3);
        this.top = new Block(posX, posY - this.grid.cellSize * 2    ,   gridX,       gridY -2  , topID);
        var nextID = Math.floor(Math.random() * 3);
        while(nextID === topID) //ensure no two blockes overlap
            nextID = Math.floor(Math.random() * 3);
        
        this.mid = new Block(posX, posY - this.grid.cellSize        ,   gridX,       gridY - 1 , nextID);        
        this.bot = new Block(posX, posY                             ,   gridX,       gridY     , Math.floor(Math.random() * 3));               
       
        //debug override
        /*       
       this.bot = new Block(posX, posY                             ,   gridX,       gridY     , 1);
       this.mid = new Block(posX, posY                             ,   gridX,       gridY  -1 , 2);
       this.top = new Block(posX, posY                             ,   gridX,       gridY  -2 , Math.floor(Math.random() * 3));
       */
      
        this.grid.PutOnGrid(this.bot);
        this.grid.PutOnGrid(this.mid);
        this.grid.PutOnGrid(this.top);
    };
    
    this.Update = function(dt)
    {
        //check if the piece has reached the end of the grid, freeze it if so
        //if(this.active && this.grid.gridYCount - 1 === this.bot.gridY )        
        //    this.LockPiece();
        
        if(this.active)
        {
            this.bot.Update(dt);
            this.mid.Update(dt);
            this.top.Update(dt);   
            
            this.movePiece();                                         
        }
    };
    
    this.MoveRight = function(dt)
    {
        this.PerformMoveRight = function()
        {
            if(this.grid.CheckSpaceRightFree(this.bot))
            {
                this.lastMoveDir = directionEnum.RIGHT;
                this.lastMoveTime = 0;
                this.RemoveFromGrid();
                
                this.bot.gridX ++;
                this.bot.sprite.x += this.grid.cellSize;                        
                this.mid.gridX ++;
                this.mid.sprite.x += this.grid.cellSize;
                this.top.gridX ++;
                this.top.sprite.x += this.grid.cellSize;            
                
                //correct sprite position
                //this.bot.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.bot.gridY);
                //this.mid.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.mid.gridY);
                //this.top.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.top.gridY);
                
                this.PutOnGrid();
            }
        };
        
        //is right in bound 
        if(this.bot.gridX + 1 < this.grid.gridXCount)
        {
            if(this.lastMoveDir !== directionEnum.RIGHT) //check if we moved left last - auto move left if not the last move
                this.PerformMoveRight();
            else
            {
                //if piece moved left last repeat after  min move time                
                if(this.lastMoveTime >= this.minMoveTime)
                    this.PerformMoveRight();
                
                this.lastMoveTime += dt;
            }
        }
    };    
    
    
    this.MoveLeft = function(dt)
    {
        this.PerformMoveLeft = function()
        {
            if(this.grid.CheckSpaceLeftFree(this.bot.gridX, this.bot.gridY))
            {
                this.lastMoveDir = directionEnum.LEFT;
                this.lastMoveTime = 0;
                this.RemoveFromGrid();
                
                this.bot.gridX --;
                this.bot.sprite.x -= this.grid.cellSize;                        
                this.mid.gridX --;
                this.mid.sprite.x -= this.grid.cellSize;
                this.top.gridX --;
                this.top.sprite.x -= this.grid.cellSize;            
                
                //correct sprite position
                //this.bot.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.bot.gridY);
                //this.mid.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.mid.gridY);
                //this.top.sprite.y = this.grid.marginTop + (this.grid.cellSize * this.top.gridY);                
                
                this.PutOnGrid();
            }
        };
        
        //is left in bound
        if(this.bot.gridX - 1 >= 0)
        {
            if(this.lastMoveDir !== directionEnum.LEFT) //check if we moved left last - auto move left if not the last move
                this.PerformMoveLeft();
            else
            {
                //if piece moved left last repeat after  min move time                
                if(this.lastMoveTime >= this.minMoveTime)
                    this.PerformMoveLeft();
                
                this.lastMoveTime += dt;
            }
        }
    };
    
    this.RotateOrder = function(dt)
    {
        //check last buttom press
        if(this.lastMoveTime >= this.minMoveTime)
        {
            this.lastMoveDir = directionEnum.ROTATE;
            this.lastMoveTime = 0;        

            //move the bottom block to top block and mid to bottom etc

            this.RemoveFromGrid();

            ///-------------move the top piece down
            //store the top pos
            var spriteTopY = this.top.sprite.y;
            var sprGridTopPos = this.top.spriteGridPos;
            var gridYTopPos = this.top.gridY;
            
            
              //move the top to the bottom
            this.top.sprite.y = this.bot.sprite.y;
            this.top.spriteGridPos = this.bot.spriteGridPos;
            this.top.gridY = this.bot.gridY;

            //store the mid pos
            var spriteMidY = this.mid.sprite.y;
            var sprGridMidPos = this.mid.spriteGridPos;
            var gridYMidPos = this.mid.gridY;      

            //move the mid piece to the top
            this.mid.sprite.y = spriteTopY;
            this.mid.spriteGridPos = sprGridTopPos;
            this.mid.gridY = gridYTopPos;

            //move the bot piece to the middle
            this.bot.sprite.y = spriteMidY;
            this.bot.spriteGridPos = sprGridMidPos;
            this.bot.gridY = gridYMidPos;      
            
            //switch the bottom and top variables around            
            var tempTop = this.top;            
            this.top = this.mid;                        
            this.mid = this.bot;
            this.bot = tempTop;
            
            this.PutOnGrid();
        }
        else
            this.lastMoveTime += dt;
    };
    
    this.LockPiece = function()
    {
        this.bot.Lock(this.grid);
        this.mid.Lock(this.grid);
        this.top.Lock(this.grid);
        
        this.active = false;
    };
    
    this.RemoveFromGrid = function()
    {
        this.grid.grid[this.bot.gridX][this.bot.gridY] = "null"; //clear the current bot position
        this.grid.grid[this.bot.gridX][this.bot.gridY -1] = "null"; //clear the current mid position
        this.grid.grid[this.bot.gridX][this.bot.gridY -2] = "null"; //clear the current top position        
    };
    
    this.PutOnGrid = function()
    {
        this.grid.grid[this.bot.gridX][this.bot.gridY] = this.bot; //clear the current position
        this.grid.grid[this.mid.gridX][this.bot.gridY -1] = this.mid; //clear the current position
        this.grid.grid[this.top.gridX][this.bot.gridY -2] = this.top; //clear the current position        
    };
    
    //moves the piece on the grid
    this.movePiece = function()
    {
        //if the bottom piece locks then lock the piece
        this.grid.MoveBlock(this.bot);
        if(!this.bot.active) //check if the block has hit something
            this.LockPiece();
        this.grid.MoveBlock(this.mid);
        this.grid.MoveBlock(this.top);                
    };
}