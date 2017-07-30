#pragma once
#include "UIBase.h"
#include <tstring.h>
#include <functional>
namespace SGA {
	class ListBox : public UIBase
	{
	public:
		typedef std::function<void(int sel, void* data)> CALLBACK_FUNC_PARAM;

		ListBox();
		virtual ~ListBox();
		virtual HRESULT init(int x, int y, int w, int h);
		virtual void release();
		virtual void update();
		virtual void render();

		void setSelectCallback(CALLBACK_FUNC_PARAM cb, void* data);

		SGA::tstring getString(int selIdx) const;
		int getSelectIdx() const;
		void insert(SGA::tstring element);
		void remove(SGA::tstring element);
		void removeAll();
		void select(int idx);
		void select(SGA::tstring element);
		virtual int getWidth() const;
		virtual int getHeight() const;
	private:
		class ListBoxCallback {
			CALLBACK_FUNC_PARAM cb;
			void* data;
		public:
			ListBoxCallback()
				:cb(NULL), data(NULL) {
			}
			void set(CALLBACK_FUNC_PARAM cb, void* data) {
				this->cb = cb;
				this->data = data;
			}
			inline void call(int sel) {
				if (cb) {
					cb(sel, data);
				}
			}
		};
	private:
		std::vector<SGA::tstring> _elements;
		ListBoxCallback _cbSelect;
		int _selectIdx;
		int _w, _h;
	};

}