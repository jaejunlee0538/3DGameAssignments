#pragma once
namespace SGA {
	class RefCount
	{
	public:
		void addReference();
		void release();

		size_t getUseCount() const;

	protected:
		RefCount();
		~RefCount();

	private:
		size_t _referenceCount;
	};

}