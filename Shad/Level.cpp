#include <string>

using namespace std;

class Level {
	public:
	
	Level(int level) {
		_level = level;
		LEVEL = "level";
		EXT = ".ext";
		filename = LEVEL +  to_string(level) + EXT;
	}

	private:
		int _level;
		string LEVEL;
		string EXT;
		string filename;
};
