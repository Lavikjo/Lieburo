#pragma once

#include <map>
#include <cassert>
#include <memory>
#include <string>
#include <stdexcept>



template <typename Resource, typename Identifier>
class ResourceHolder {

public:
	void load(Identifier id, const std::string &filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;	
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename) {
	std::unique_ptr<Resource> resource(new Resource());
	if(!resource->loadFromFile(filename)) {
		throw std::runtime_error("Failed to load " + filename);
	}

	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}