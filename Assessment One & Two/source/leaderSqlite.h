#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "sqlite3.h"
#include "drawEngine.h"
#include "level.h"
#include "sprite.h"
#include <fstream>
#include <iostream>
#include <array>

#include <string>

struct DBLeaderBoard
{
	//std::array<std::array<int , 10>, 10> score;
	//std::array<int, 10> id;	
	//std::string name[10];	
	int score[10];
	int id[10];
	std::string name[10];	
};


//todo
//inherit from sprite to get the drawing ability
//: Sprite
class LeaderDB 
{
public:
	
	LeaderDB(DrawEngine *De,Level *le, int width, int height);
	//descontructor
	~LeaderDB();

	//insert multiple scores into DB
	//mainly for generating and debugging DB/tables
	void insertMultiToDB(const char *arg_stmt);

	//for single score only
	//for multi scores use insertMultiToDB
	bool insertNewScoreToDB(std::string name, int score);

	//update exisisting score at dbID position
	bool updateScore(std::string name, int score, int dbID);
	
	//create table
	//use in conjuntion with insertMultiToDB to create
	//a new populated database
	bool createTable();
	
	//retreive the entire leaderboard for display
	DBLeaderBoard getLeaderBoard();

	std::string getLeaderBoardStr();

	//std::string& operator= (const std::string& str);
	
	//DBLeaderBoard operator =(std::string &st);

	//if a leader table is in exisitence this does nothing.
	//otherwise it generates the leader table in the DB for use
	void generateTable();

	void drawBoard(DBLeaderBoard ldrBd);

protected:

private:
	DrawEngine *dEngine;		//game's draw engine
	Level *gameLevel;
	sqlite3 *db;				//leaderboard database
	//sqlite3_stmt *sqlStatment;	//current statment being processed

	DBLeaderBoard Struct_leaderBoard;

	const char *dbName;			//databse name
	const char * dbTable;		//game's score table

	bool connectToDB();

	int leaderScreenWidth, leaderScreenHeight;
	
};



#endif