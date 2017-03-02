#ifndef COMPONENT_H
#define COMPONENT_H

enum TypeComponent
{
	TRANSFORM = 0,
};

class Component
{

public:
	Component() {}
	virtual ~Component() {}

	virtual void Enable() {
		enable = true;
	}
	virtual bool OnUpdate() { return true; }
	virtual bool OnEditor() { return true; }
	virtual void Disable() {
		enable = false;
	}


private: 
	TypeComponent type;
	bool enable;
};

#endif

