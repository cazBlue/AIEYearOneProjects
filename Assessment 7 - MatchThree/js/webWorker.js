var blockEnum = 
{
    GREEEN  : 0,
    RED     : 1,
    BLUE    : 2    
};


onmessage = function (oEvent) {   
    //worker that builds a string based on how many blocks there are
    
    var obj = JSON.parse(oEvent.data);        
    
    var grid = obj.grid;
    
    var lengthX = obj.gridXLen;
    var lengthY = obj.gridYLen;
    
    var result = "";
    
    var countRed = 0;
    var countGreen = 0;
    var countBlue = 0;
    
    for(var y = 0; y < lengthY; ++y) //populate the array with block types
    {
        for(var x = 0; x < lengthX; ++x)
        {
            switch(grid[x][y])
            {
                case blockEnum.BLUE:
                    //postMessage("blue block found");
                    countBlue++;
                    break;
                case blockEnum.GREEEN:
                    //postMessage("green block found");
                    countGreen++;
                    break;
                case blockEnum.RED:
                    //postMessage("red block found");                
                    countRed++;
                    break;
                default:
                    //postMessage("not a block");
            }
            //postMessage("looking at " + x + ", " + y + " and it is a " + grid[x][y]);
        }
    }
    
    result = "Worker said:  <br />" +  countBlue + " blue blocks<br />" + countGreen + " green blocks <br />" + countRed + " red blocks";
    
    postMessage(result);
    
  //postMessage("Hi " + oEvent.data);
};

//TODO work out how much probablity of a certain type of block their is, or show stats on what blocks are in what line