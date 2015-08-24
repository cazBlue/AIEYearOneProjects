/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


//web worker loads the weather!
//reference : http://www.w3schools.com/html/html5_webworkers.asp

function WebWorker(a_grid)
{
    this.grid = a_grid;
    
    //var worker;

    this.myWorker = new Worker("js/webWorker.js");      
    
    this.init = function()
    {
        
    };
    

    this.myWorker.onmessage = function (oEvent) {
      //console.log("Worker said : " + oEvent.data);
      
      document.getElementById("blockUpdater").innerHTML = oEvent.data;
      
    };
    
    
    this.DisplayBlockCount = function ()
    {
            //start the web worker passing the clean grid
            
        var workerObj = new createWorkerObj(this.grid);

        var stringed = JSON.stringify(workerObj);                    
            
        this.myWorker.postMessage(stringed);
    };    
};


function createWorkerObj(a_grid)
{
    //create a worker object with the following:
    //clean grid (grid holds block types - or array of block types
// ----- to avoid cyclic errors (JSON.stringify) and clone errors when passing the grid array to the web worker we build a clean array
// ----- this causes an overhead but it's unavoidable as it has to be parsed to be passed (hahaha)
    this.grid =  [];       
    for(var i = 0; i < a_grid.gridXCount; ++i)//set the grid array up, grid is defined by y columns    
        this.grid[i] = new Array();    

    for(var y = 0; y < a_grid.gridYCount; ++y )
    {
        for(var x = 0; x < a_grid.gridXCount; ++x )
        {   
            if(a_grid.grid[x][y] instanceof Block) 
                this.grid[x][y] = a_grid.grid[x][y].type;            
            else
                this.grid[x][y] = null;
        }
    }        

    this.score = a_grid.score;

    this.gridXLen = a_grid.gridXCount;
    this.gridYLen = a_grid.gridYCount;

    //current score

    //list of high scores       

    //return this;
};