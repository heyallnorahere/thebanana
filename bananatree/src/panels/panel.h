#pragma once
namespace bananatree {
	class panel {
	public:
		virtual void update();
		virtual void render() = 0;
		virtual ~panel();
	};
}