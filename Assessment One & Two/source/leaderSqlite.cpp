#include "leaderSqlite.h"
#include <sstream>


//default constructor
LeaderDB::LeaderDB(DrawEngine *arg_De,Level *arg_level,  int arg_width, int arg_height)
{	
	//db ;		//handle to the active database
	//dEngine = gameDe;	//handle to the games draw engine
	dbName = "leaderBoard";	//databse name
	dbTable = "scores";	//table name containing scores

	dEngine = arg_De;
	gameLevel = arg_level;

	leaderScreenWidth = arg_width;
	leaderScreenHeight = arg_height;
	//open the database for use.
	connectToDB();
}

std::string LeaderDB::getLeaderBoardStr()
{
	std::stringstream ss;
	DBLeaderBoard curBoard = getLeaderBoard();

	for(int i = 0; i < 10; i++)
	{
		if(curBoard.name[i] != "")
			ss << "Name: " << curBoard.name[i] << " - Score: " <<  curBoard.score[i] << "\n";
	}
	
	return ss.str();
}

LeaderDB::~LeaderDB()
{
	//dump the leaderboard to a text file when the game ends
	std::fstream file;		
	std::string curBoard;

	curBoard = getLeaderBoardStr();

	file.open("human_readable_leaderBoard.txt", std::ios_base::out); 
		
	file << curBoard << std::endl;

	file.close();
}



void LeaderDB::insertMultiToDB(const char *stmt)
{
		/*------------------ insert into DB ---------------------- */
		sqlite3_stmt *sqlStatment;
		const char * leftOver = 0;	//used to contain the tail of the insert statement for multi entries

		int result;	//result of prepare operation

		result = sqlite3_prepare_v2(db, stmt, strlen(stmt) +1, &sqlStatment, &leftOver );	//initial statement prep for while loop
		
		//loop while there is statments to insert into DB
		//
		while(sqlite3_step(sqlStatment) != SQLITE_MISUSE)
		{
			int result = sqlite3_prepare_v2(db, stmt, strlen(stmt) +1, &sqlStatment, &leftOver );
			
			stmt = leftOver;//set the statment to the left over, if none the loop will throw an error and exit
		}

		//return entryAdded;
		
		/* ---------------  end insert into DB ------------------------ */

}

bool LeaderDB::insertNewScoreToDB(std::string name, int score)
{
	int result;	//result of prepare operation
	sqlite3_stmt *sqlStatment;

	std::stringstream ss;
	ss << "INSERT INTO Leaderboard (NAME,SCORE) " << "VALUES ('" << name << "', "<< score << "); " ;
	std::string s = ss.str();

	const char* stmt = s.c_str();
	
	result = sqlite3_prepare_v2(db, stmt, strlen(stmt) +1, &sqlStatment, NULL );

	if(result == SQLITE_OK)
		result = sqlite3_step(sqlStatment);

	return false;
}

bool LeaderDB::createTable()
{
	sqlite3_stmt *sqlStatment;
	const char *sqlCreateTable = "CREATE TABLE leaderboard("  \
					"ID				INTEGER PRIMARY		KEY," \
					"NAME           TEXT				NOT NULL," \
					"SCORE          INT					NOT NULL);";
	int result = sqlite3_prepare_v2(db, sqlCreateTable, strlen(sqlCreateTable) +1, &sqlStatment, NULL );
	if(SQLITE_OK == result )
	{
		sqlite3_step(sqlStatment);
		return true;
	}

	return false;
}

bool LeaderDB::updateScore(std::string arg_name, int arg_score, int arg_dbID)
{
	//todo add update score

	sqlite3_stmt *sqlStmt;
	std::string stmtPrep = "UPDATE leaderboard SET NAME='" + arg_name + "', SCORE=" + std::to_string(arg_score) +" WHERE ID = "+ std::to_string(arg_dbID) +" ";
	const char *sqlPrep = stmtPrep.c_str();

	int result = sqlite3_prepare_v2(db, sqlPrep, strlen(sqlPrep) +1, &sqlStmt, NULL );

	if(SQLITE_OK == result )
	{
		sqlite3_step(sqlStmt);
		drawBoard(getLeaderBoard());
		return true;
	}

	return false;
}

DBLeaderBoard LeaderDB::getLeaderBoard()
{
	DBLeaderBoard leaderboard;
	sqlite3_stmt *sqlStatment;

	const char * sqlSelect = "SELECT ID, NAME, SCORE from leaderboard ORDER BY SCORE DESC;";
	const char * sqlLeftOver = 0;	//used to contain the tail of the insert statement for multi entries
	//////////////////------------------ iterate through results
			
	int col = 0;

	if(SQLITE_OK == sqlite3_prepare_v2(db, sqlSelect, strlen(sqlSelect) +1, &sqlStatment, &sqlLeftOver ))
	{	
		while (1) {
			int s;
			
			s = sqlite3_step (sqlStatment);
			
			if (s == SQLITE_ROW && col < 10) {
				int bytes, sqType;
				const unsigned char * text;
				//bytes = sqlite3_column_bytes(sqlStatment, 1);
				
				for(int i = 0; i <  sqlite3_column_count(sqlStatment); i++)
				{					
					switch (i)
					{
					case 0:
						leaderboard.id[col] = sqlite3_column_int (sqlStatment, i);
					case 1:
						leaderboard.name[col] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(sqlStatment, i)));					
					case 2:
						leaderboard.score[col] = sqlite3_column_int (sqlStatment, i);
					default:
						break;
					}
					//leaderboard  += std::string(reinterpret_cast<const char*>(sqlite3_column_text (sqlStatment, i))) + " ";
					//sqType = sqlite3_column_type(sqlStatment, i);
					int colNum = sqlite3_column_count(sqlStatment) - 1;
					if(colNum == i)
					{
						col++;
					}
				}
			}
			else {
				break;
			}
		}
	}		
	//--------------------------------------------end result iteration

	return leaderboard;
}

bool LeaderDB::connectToDB()
{
	int result = sqlite3_open(dbName, &db);
	if (SQLITE_OK  == result )
		return true;
	else
		return false;	
}

void LeaderDB::drawBoard(DBLeaderBoard arg_board)
{
	//dEngine->
	int mapWidth = dEngine->getScreenWidth() + 2;
	int startLoc = mapWidth; //remember where the map starting point is
	int increment = 0;
	int y = 1;
	int counter = 0;
	std::string toPrint;	
	std::string headerMessage = "------Leader Board------";
	//char escpOne = '\\';
	//char escTwo = 'n';


	for (int x = 0; x < headerMessage.length(); x++)
	{
		dEngine->drawHud(headerMessage.at(x), mapWidth  + increment, y);

		increment ++;
	}
	
	increment = 3;
	y += 2;

	for (int i = 0; i < 10 ; i++)
	{
		//if(mapWidth + increment - 1 >= leaderScreenWidth + mapWidth || arg_string.at(i) == escpOne && arg_string.at(i + 1) == escTwo)
		if(mapWidth + increment - 1 >= leaderScreenWidth + mapWidth || counter > 0)
		{
			mapWidth = startLoc;
			increment = 3;
			y++;
			counter = 0;
			
		}
		
		std::stringstream ss;
		ss << (i+1) << " - " << arg_board.name[i] << " - " << arg_board.score[i];
		
		toPrint = ss.str();
		
		
		for (int x = 0; x < toPrint.length(); x++)
		{
			dEngine->drawHud(toPrint.at(x), mapWidth  + increment, y);

			increment ++;
		}
		counter ++;
	}		
}

void LeaderDB::generateTable()
{
	
	if(createTable())
	{
		//build a default scoreboard for the player
		insertMultiToDB(	\
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (0, 'Kit', 180 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (1, 'guran', 187 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (2, 'Ghost', 150 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (3, 'Phantom', 300 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (4, 'michiru', 201 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (5, 'haruka', 160 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (6, 'hotaru', 70 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (7, 'gotham', 40 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (8, 'minako', 65 ); " \
			"INSERT INTO Leaderboard (ID,NAME,SCORE) "  \
			"VALUES (9, 'rei', 33 ); "
			);	
	}
}




/*
//create table reference only

	sqlCreateTable = "CREATE TABLE COMPANY("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "AGE            INT     NOT NULL," \
         "ADDRESS        CHAR(50)," \
         "SALARY         REAL );";

*/

/*	
//insert reference only

	sqlInsert = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

*/


//other reference code from game.cpp testing



	
//	if (SQLITE_OK  == (sqlite3_open(dataBase, &leaderDB)))
//	{	
//		int resultClose, resultStep, resultFinalize, resultCount;	//results of SQL operations
//		
//		
//
//
//
//		if(SQLITE_OK == sqlite3_prepare_v2(leaderDB, sqlSelect, strlen(sqlSelect) +1, &sqlStatment, &sqlLeftOver ))
//		{			
//			//cout << "statment prepared" << endl;
//			//prepared statment was ok
//			
//			resultStep = sqlite3_step(sqlStatment);	//single statment
//
//			
//
//			//resultCount = sqlite3_column_count(sqlStatment);
//			
//			/*
//			while(resultStep == SQLITE_ROW)
//			{
//				const unsigned char * text;
//				text = sqlite3_column_text(sqlStatment, 0);	
//				
//				resultStep = sqlite3_step(sqlStatment);
//			}
//			*/
//
//
//			//finalise the statments to avoid clashes
//			resultFinalize = sqlite3_finalize(sqlStatment);
//		}
//		else
//		{
//			//cout << "statment not prepared " << result << endl;		
//			//problem with the incoming statement
//		}
//		//fprintf
//		resultClose = sqlite3_close_v2(leaderDB);
//	}
//	else
//	{
//		//database was not created or opened
//	}
//	
