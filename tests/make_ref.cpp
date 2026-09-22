#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	string line;

	while (getline(fin, line)) {
		size_t semi = line.find(";-");
		if (semi != string::npos) {
			string hex = line.substr(semi + 2);

			istringstream hin(hex);
			string byte_str;
			while (getline(hin, byte_str, ' ')) {
				unsigned char byte = stoi(byte_str, nullptr, 16);
				fout.put(byte);
			}
		}
	}

	fout.close();
	fin.close();

	return 0;
}
