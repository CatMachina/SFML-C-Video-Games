#include <map>
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class TextureHolder
{
private:
	map<string, Texture> m_Textures;

	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static Texture& getTexture(string const& filename);
};