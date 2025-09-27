//
// Chaos Simulation : classes.hpp
// Designed and Coded by Captain Bear
//

// Includes
#include <string>

// Classes
// Basic Classes
class Object_public {
private:
	std::string Name;
	long long ObjectID_High, ObjectID_Low;
public:
	GetObjectInfo() {
		std::cout << "Name: " << Name << std::endl
			<< "Object ID: " << ObjectID_High << "." << ObjectID_Low
	}
};
class Object_private {
	std::string Name;
	long long ObjectID_High, ObjectID_Low;
};

// Entity Right Classes
class Entity_public : public Object_public, private Object_private {
private:
	std::string AI_UserName;
	long long AI_ID_High, AI_ID_Low;
public:
	GetAIBrainInfo() {
		std::cout << "AI User Name: " << AI_UserName << std::endl
			<< "AI ID: " << AI_ID_High << "." < AI_ID_Low << std::endl;
	}
};
class Entity_private : public Object_private {
	std::string AI_UserName;
	long long AI_ID_High, AI_ID_Low;
};

class Glitch_public : public Entity_public, private Entity_private {
	//
};
class Glitch_private : public Entity_private {
	//
};

class God_public : public Glitch_public, private Glitch_private {
	//
};
class God_private : public Glitch_private {
	//
};

class Admin_public : public God_public, private God_private {
	//
};
class Admin_private : public God_private {
	//
};

class RealityGlitchClass1_public : public Admin_public, private Admin_private {
	//
};
class RealityGlitchClass1_private : public Admin_private {
	//
};

class RealityGlitchClass2_public : public RealityGlitchClass1_public, private RealityGlitchClass1_private {
	//
};
class RealityGlitchClass2_private : public RealityGlitchClass1_private {
	//
};

class RealityGlitchClass3 : public RealityGlitchClass2_public, private RealityGlitchClass2_private {
	//
};