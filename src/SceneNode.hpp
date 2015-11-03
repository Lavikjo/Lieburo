#ifndef SCENENODE_HH
#define SCENENODE_HH

class SceneNode {

public:
	typedef std::unique_ptr<SceneNode> Ptr;

	SceneNode();

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

private:
	std::vector<Ptr> mChildren;
	SceneNode* mParent;
};


#endif