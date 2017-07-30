#pragma once
#include <Windows.h>
#include <vector>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(linker, "/HEAP:25000000")
namespace SGA {

	class BitmapImage
	{
		union ARGB {
			struct tagARGB {
				BYTE b, g, r, a;
			};
			tagARGB rgb;
			DWORD data;
		};

		typedef struct tagImageInfo {
			HDC hMemDC;
			HBITMAP hBit;
			HBITMAP hOBit;
			int width;
			int height;
			int BPP;//Bits per Pixel
			BITMAPINFO bmpInfo;

			tagImageInfo();
		} IMAGE_INFO, *LPIMAGE_INFO;
	public:
		enum TransparentModes {
			TRANSPARENT_MODE_NONE,
			TRANSPARENT_MODE_ALPHA,
			TRANSPARENT_MODE_KEYCOLOR
		};

		BitmapImage();
		virtual ~BitmapImage();

		static void setHWND(HWND hWnd);
		/*
		source의 좌우가 반전된 비트맵을 가진 BitmapImage객체를 생성한다.
		*/
		BitmapImage* createMirroredImage() const;

		HRESULT init(int width, int height, TransparentModes transparentMode = TRANSPARENT_MODE_NONE, COLORREF keyColor = RGB(255, 0, 255));
		HRESULT init(const TCHAR* fileName, int width, int height, TransparentModes transparentMode, COLORREF keyColor = RGB(255, 0, 255));
		HRESULT init(const TCHAR* fileName, TransparentModes transparentMode, COLORREF keyColor = RGB(255, 0, 255));

		void release();

		void render(HDC hdc, int destX, int destY)  const;
		void render(HDC hdc, int destX, int destY, int destWidth, int destHeight) const;
		void render(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight) const;
		void render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight) const;
		void renderRot(HDC hdc, int destX, int destY, float angleRad, int pivotX, int pivotY) const;
		void renderRot(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, float angleRad, int pivotX, int pivotY) const;
		void renderAlpha(HDC hdc, int destX, int destY, BYTE alpha) const;
		void renderAlpha(HDC hdc, int destX, int destY, int destWidth, int destHeight, BYTE alpha) const;
		void renderAlpha(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) const;
		void renderAlpha(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) const;

		void clearImage();
		/*
		이미지를 color색으로 칠한다.
		*/
		void clearImage(COLORREF color);

		inline HDC getDC() const {
			return _imageInfo->hMemDC;
		}

		inline BOOL isInitialized() const {
			return _imageInfo != NULL;
		}

		inline TransparentModes getTransparentMode() const {
			return _transMode;
		}

		inline COLORREF getKeyColor() const {
			return _keyColor;
		}

		//inline void enableTransparent(BOOL enable) {
		//	
		//}

		inline int getWidth() const {
			return _imageInfo->width;
		}

		inline int getHeight() const {
			return _imageInfo->height;
		}

		COLORREF BitmapImage::getPixel(int ix, int iy) const;

		void BitmapImage::setPixel(int ix, int iy, COLORREF color);

		void getPixelData(std::vector<DWORD>& pixels) const;
	private:
		void multiplyRGBWithAlpha(std::vector<DWORD>& pixels);

		//복사 방지
		BitmapImage(const BitmapImage& other) {}
		void initMaskImage() const;
		void releaseImage(LPIMAGE_INFO img);
		void initXMirroredImage() const;
	private:
		LPIMAGE_INFO _imageInfo;
		mutable LPIMAGE_INFO _maskImageInfo;

		//BOOL _isTransparentEnabled;
		COLORREF _keyColor;

		TransparentModes _transMode;

		mutable BitmapImage * _xMirrored;
		//bool _hasAlpha;
		static HWND _hWnd;
	};
}
