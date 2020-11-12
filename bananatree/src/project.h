#pragma once
namespace bananatree {
	class project {
	public:
		project();
		void rename(const std::string& name);
		void reset();
		void save(const std::string& path);
		void load(const std::string& path);
	private:
		std::string m_name;
	};
}