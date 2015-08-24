/* 
 * Author:Callan Winfield - callanw.com
 */

var g_width = 800;
var g_height = 600;

var game = new Phaser.Game(g_width, g_height, Phaser.AUTO, 'match3Game', { preload: preload, create: create, update: update });

function preload() {
    game.load.image('arrowGreen', 'assets/arrow_green.png');
    game.load.image('arrowBlue', 'assets/arrow_blue.png');
    game.load.image('arrowRed', 'assets/arrow_red.png');
}

    function create() {    

        //set background colour
        game.stage.backgroundColor = '#535353';

        //create the master grid, note that the cell size should match the dimensions of a block sprite
        this.masterGrid = new grid(50, 40,  20, 15, 32);

        //TODO put states in splash etc
        this.masterGrid.Create();

        this.masterGrid.init();  //kick off the game       
        
        //this.webWorker = new WebWorker(this.masterGrid);
        //this.webWorker.init();        
        
        //this.webWorker.DisplayBlockCount();
        
}

function update() {        
    //main game loop    
    this.masterGrid.Update(game.time.physicsElapsed);    //update the grid - does not handle user input
}