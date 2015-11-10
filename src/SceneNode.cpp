void SceneNode::attachChild(Ptr child) {

	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {

	auto found = std::find_if(mChildren.begin(), mChildren.end(),
		[&] (Ptr& p) -> bool { return p.get() == &node; });

	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	
	states.transform *= getTransform();

	drawCurrent(target, states);

	for(auto child : mChildren) {
		child->draw(target, states);
	}
}

void SceneNode::update(sf::Time deltaTime) {
	updateCurrent(deltaTime);

	for(auto child : mChildren) {
		child->update(deltaTime);
	}

}