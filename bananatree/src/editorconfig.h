#pragma once
namespace bananatree {
	class editorconfig {
	public:
		template<typename T> void set(const std::string& key, const T& value);
		template<typename T> const T& get(const std::string& key);
		void load(const std::string& path);
		void save(const std::string& path);
	};
}