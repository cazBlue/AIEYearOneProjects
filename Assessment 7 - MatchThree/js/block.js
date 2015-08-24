/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


//single block, belongs inside a piece. Update called from block

//CREATE DIFFERENT STATES FOR THE BLOCK

var blockEnum = 
{
    GREEEN  : 0,
    RED     : 1,
    BLUE    : 2    
};

function Block (posX, posY, gridX, gridY, a_type) {    
    
    //set the type of block and create sprite
    //var type = Math.floor(Math.random() * 3);
    this.type = a_type;

    switch(this.type)
    {
        case blockEnum.BLUE:
            this.sprite = game.add.sprite(posX, posY, 'arrowBlue');
            break;
        case blockEnum.GREEEN:
            this.sprite = game.add.sprite(posX, posY, 'arrowGreen');
            break;           
        case blockEnum.RED:
            this.sprite = game.add.sprite(posX, posY, 'arrowRed');
            break;                   
        default:
            console.log("error finding block type");
            break;                   
    }
    
    this.sprite.visible = false;
    
    this.gridX = gridX;
    this.gridY = gridY;
    
    this.spriteGridPos = posY; //yPos of block on grid space update
    this.active = true; //if active this piece will move
    this.dirty = false; //dirty
    
    this.Update = function(dt) 
    {
        if(this.active)                
            this.sprite.y += 150 * dt;
        
        if(this.gridY >= 0)    //if the block is moved back outside the grid make it invisible 
            this.sprite.visible = true;
        else
            this.sprite.visible = false;
    };
    
    this.UpdateHeavy = function(dt, grid)
    {
        if(this.active)
        {
            this.sprite.y += 150 * dt;        
                    
            grid.MoveBlock(this);                         
        }
    };
      
    this.Lock = function(grid)
    {
        //push the sprite into perfect alignment
        this.sprite.y = grid.marginTop + (grid.cellSize * this.gridY);
        
        this.active = false;
        
        if(this.gridY >= 0)    //make sure the piece is visible
            this.sprite.visible = true;        
    };

    this.UnLock = function()
    {        
        this.active = true;
    };
}