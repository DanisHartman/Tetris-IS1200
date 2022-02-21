console.log("hello");

var c = document.getElementById("canvas");
var ctx = c.getContext("2d");
ctx.scale(20,20);

var matrix = [];
	

function shapes(type) {
	if(type === "T"){
		return [
			[1,1,1],
			[0,1,0],
			[0,0,0],
		];
	}
	
	if(type === "O"){
		return [
			[1,1,0],
			[1,1,0],
			[0,0,0],
		];
	}
	if(type === "L"){
		return[
			[0,1,0,0],
			[0,1,0,0],
			[0,1,0,0],
			[0,1,0,0],
		];
	}

	if(type === "Z") {
		return[
			[1,1,0],
			[0,1,1],
			[0,0,0],
		];
	}

	if(type === "J"){
		return[
			[0,1,0],
			[0,1,0],
			[1,1,0],
		];
	}

	if(type === "S"){
		return[
			[0,1,1],
			[1,1,0],
			[0,0,0],
		];
	}
}

var arena = [];

function generateArenaMatrix(rows, cols) {
	var x = new Array(rows);

	for (var i = 0; i < rows; i++) {
		x[i] = new Array(cols);
	}

	for (var d = 0; d < rows; d++) {
		for (var y = 0; y < cols; y++) {
			x[d][y] = 0;
		}
	}
	return x;
}

arena = generateArenaMatrix(40,20);
console.log(arena);

function drawMatrix(matrix, pos) {
	var matrixLength = matrix.length;
	for (var i = 0; i < matrixLength; i++) {
		for (var j = 0; j < matrixLength; j++) {
			if(matrix[i][j] === 1)
			{
				var colorArr = ["red", "green", "blue", "yellow", "orange", "pink", "purple"];
				ctx.fillStyle = colorArr[player.color];
				ctx.fillRect(j + pos.x,i + pos.y,0.9, 0.9)
			}
		}		
	}
}

function clear() {
		console.log("Hej2");
	outer: for (var y = arena.length - 1; y > 0; y--) {
		for (var x = 0; x < arena[y].length; x++) {
			if (arena[y][x] === 0){
				continue outer;
			}
		}
		const row = arena.splice(y,1)[0].fill(0);
		arena.unshift(row);
		++y;
	}

}

function drawArena(arena) {
	for (var y = 0; y < arena.length; y++) {
		for (var x = 0; x < arena[y].length; x++) {
			if(arena[y][x] !== 0)
			{
				var colorArr = ["red", "green", "blue", "yellow", "orange", "pink", "purple"];
				ctx.fillStyle = colorArr[arena[y][x]-1];
				ctx.fillRect(x,y,0.9, 0.9);
			}
		}		
	}
}

function addToArena() {
	for (var y = 0; y < player.pMatrix.length; y++) {
		for (var x = 0; x < player.pMatrix[y].length; x++) {
			if(player.pMatrix[y][x] !== 0) {
				arena[player.pos.y + y ][player.pos.x + x] = player.color+1; 
			}
		}
	}
}

function main() {
	requestAnimationFrame(update);
}
console.log(shapes("T"));
var player = {
	pMatrix : shapes("T"),
	pos : {x:((canvas.width/20)/2)-2, y: 0},
	color : 1
}

function rotateMatrix() {
	var temp= [
			[0,0,0],
			[0,0,0],
			[0,0,0]];
			

	temp = transpose(player.pMatrix, temp);
	console.log(temp);
	for (var i = 0; i < temp.length; i++) {
		temp[i].reverse();
	}
	//temp.reverse();
	player.pMatrix = temp;
}

 function transpose(matrix, temp)
{
    var i, j;
    for (i = 0; i < matrix.length; i++)
        for (j = 0; j < matrix.length; j++)
            temp[i][j] = matrix[j][i];
        
        return temp;
}

function drawGraphics(){
	ctx.clearRect(0,0,canvas.width,canvas.height);
	drawArena(arena);
	drawMatrix(player.pMatrix, player.pos);
}

var dropInterval = 1000;
var dropCounter = 0;
var lastTime = 0;

// Timer function 
function update(time = 0) {

	const deltaTime = time - lastTime;
	lastTime = time;
 	dropCounter += deltaTime;

 	if(dropCounter > dropInterval)
 		playerDrop();

 	drawGraphics();
 	requestAnimationFrame(update);
}

function playerDrop() {
	player.pos.y++;
	if(collision()) {
		player.pos.y--;
		addToArena();
		pieceReset();
		clear();
	}
	dropCounter = 0;
}

function pieceReset() {
	var colorValue = Math.floor(Math.random() * 7);
	var pieces = 'JLOZST';
	console.log(player.pMatrix = shapes(pieces[pieces.length * Math.random() | 0]));
	
	player.pos = {x:((canvas.width/20)/2)-2, y: 0};
	player.color = colorValue;
}

function playerMove(value) {
	player.pos.x += value;

	if(collision()){
		player.pos.x -= value;
	}
}

function collision() {
	const [m,o] = [player.pMatrix, player.pos];
	for (var y = 0; y < player.pMatrix.length; y++) {
		for (var x = 0; x < player.pMatrix[y].length; x++) {
			if(m[y][x] !== 0 &&
				(arena[y + o.y] &&
				arena[y + o.y][x + o.x]) !== 0) {
				return true;
			}	
		}
	}
	return false;
}

document.addEventListener('keydown', function(event) {
    if(event.keyCode == 37) {
        playerMove(-1);
    }
    else if(event.keyCode == 39) {
       	playerMove(1);
    }
    else if(event.keyCode == 40) {
        playerDrop();
    }
    else if(event.keyCode == 38) {
        rotateMatrix();
    }
    
    drawGraphics();
});

main();