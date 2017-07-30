#include "BitmapImage.h"
#include <assert.h>
#include <tchar.h>
#include "MathUtils.h"
#include "CommonMacroHeader.h"
#include <vector>
#include <bitset>
namespace SGA {
	HWND BitmapImage::_hWnd = 0;

	BitmapImage::tagImageInfo::tagImageInfo() {
		hMemDC = NULL;
		hBit = NULL;
		hOBit = NULL;
		width = 0;
		height = 0;
		BPP = 0;
		bmpInfo.bmiHeader.biBitCount = 0;
		bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	}

	BitmapImage::BitmapImage()
		:_imageInfo(NULL), _xMirrored(NULL), _maskImageInfo(NULL)
	{
		//비트맵 객체 생성하기 전에 Window Handle을 반드시 세팅할 것
		//BitmapImage::setHWND(hWnd);
		assert(BitmapImage::_hWnd != 0);
	}

	BitmapImage::~BitmapImage()
	{
		//반드시 수동으로 release할것!
		//파괴자에서 하게 할까?
		//assert(_imageInfo == 0);
		release();
	}

	void BitmapImage::setHWND(HWND hWnd)
	{
		BitmapImage::_hWnd = hWnd;
	}

	BitmapImage * BitmapImage::createMirroredImage() const
	{
		BitmapImage* mirroredImage = new BitmapImage();
		assert(mirroredImage->init(this->getWidth(), this->getHeight(), _transMode, _keyColor) == S_OK);
		//SelectObject(getDC(), _imageInfo->hBit);
		int width = getWidth(), height = getHeight();
		std::vector<DWORD> pixels;
		getPixelData(pixels);
		int nIterX = width / 2;
		for (int ih = 0; ih < height; ++ih) {
			DWORD tmp;
			DWORD* px = &pixels[ih*width];
			for (int iw = 0; iw < nIterX; ++iw) {
				tmp = px[iw];
				px[iw] = px[width - iw -1];
				px[width - iw - 1] = tmp;
			}
		}
		
		assert(SetDIBits(mirroredImage->getDC(), mirroredImage->_imageInfo->hBit, 0, height, &pixels[0], &(_imageInfo->bmpInfo), DIB_RGB_COLORS) != 0);
		//픽셀을 업데이트 한 후에 미러 이미지의 비트맵을 Select해주지 않으면 화면에 출력되지 않음 - 왜?
		SelectObject(mirroredImage->getDC(), mirroredImage->_imageInfo->hBit);
	/*	BOOL ret = StretchBlt(mirroredImage->getDC(),
			mirroredImage->getWidth() - 1, 0,
			-mirroredImage->getWidth(), mirroredImage->getHeight(),
			getDC(),
			0, 0,
			mirroredImage->getWidth(), mirroredImage->getHeight(),
			SRCCOPY);*/

		return mirroredImage;
	}

	HRESULT BitmapImage::init(int width, int height, TransparentModes transparentMode, COLORREF keyColor)
	{
		//재할당 방지
		if (_imageInfo != NULL) release();

		//DC 얻어온다
		HDC hdc = GetDC(BitmapImage::_hWnd);

		_imageInfo = new IMAGE_INFO;
		//비트맵 출력을 위해서 메모리 DC를 만들어 준다
		_imageInfo->hMemDC = CreateCompatibleDC(hdc);
		_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
		//_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//초기화 완료후 Select

		//이미지 초기화에 실패하면..
		if (_imageInfo->hBit == NULL) {
			release();
			return E_FAIL;
		}

		_imageInfo->width = width;
		_imageInfo->height = height;

		//BITMAPINFO 구조체 채우기
		GetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, height, NULL, &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		_imageInfo->bmpInfo.bmiHeader.biCompression = BI_RGB;
#if 0
		std::vector<DWORD> pixels(width* height);
		bool isAlphaUsed = false;
		int ret = GetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, height, &pixels[0], &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		int nPixels = width*height;
		for (int i = 0; i < nPixels; ++i) {
			pixels[i] = 0xffffffff;
			//pixels[i] = 0xff000000;
		}
		SetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, height, &pixels[0], &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
#endif
		BITMAP bmp = { 0 };
		GetObject(_imageInfo->hBit, sizeof(bmp), &bmp);
		_imageInfo->BPP = bmp.bmBitsPixel;

		//얻은 DC 해제한다
		ReleaseDC(_hWnd, hdc);

		//투명 컬러 셋팅!
		if (transparentMode == TRANSPARENT_MODE_ALPHA) {
			transparentMode = TRANSPARENT_MODE_NONE;
		}
		_transMode = transparentMode;
		_keyColor = keyColor;

		return S_OK;
	}

	HRESULT BitmapImage::init(const TCHAR * fileName, int width, int height, TransparentModes transparentMode, COLORREF keyColor)
	{
		//경로값이 없으면
		if (fileName == NULL) return E_FAIL;

		//재할당 방지
		if (_imageInfo != NULL) release();

		HDC hdc = GetDC(BitmapImage::_hWnd);
		_imageInfo = new IMAGE_INFO();
		//비트맵 출력을 위해서 메모리 DC를 만들어 준다.
		_imageInfo->hMemDC = CreateCompatibleDC(hdc);
		_imageInfo->hBit = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
		//_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);//초기화 완료후 Select
		_imageInfo->width = width;
		_imageInfo->height = height;

		//이미지 초기화에 실패하면
		if (_imageInfo->hBit == NULL) {
			release();
			return E_FAIL;
		}

		BITMAP bmp = { 0 };
		GetObject(_imageInfo->hBit, sizeof(bmp), &bmp);
		_imageInfo->BPP = bmp.bmBitsPixel;

		//BITMAPINFO 구조체 채우기
		GetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, height, NULL, &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		_imageInfo->bmpInfo.bmiHeader.biCompression = BI_RGB;

		if (transparentMode == TRANSPARENT_MODE_ALPHA) {
			////픽셀 데이터를 로드한다.
			std::vector<DWORD> pixels(width * height);
			getPixelData(pixels);

			////로드한 비트맵 파일이 pixel별 알파값을 가지고 있는지 확인한다.
			//int nPixels = bmp.bmWidth*bmp.bmHeight;
			//ARGB* px = (ARGB*)&pixels[0];
			//BYTE sampleAlpha = px->rgb.a; //모든 픽셀이 동일한 알파값을 가지고 있는지 확인
			//_hasAlpha = false;
			//for (int i = 0; i < nPixels; ++i, ++px) {
			//	if (px->rgb.a != sampleAlpha) {
			//		_hasAlpha = true;
			//		break;
			//	}
			//}

			//픽셀별 Alpha값을 가지고 있다면 모든 픽셀의 RGB값에 각각의 alpha값을 곱한다.
			//https://msdn.microsoft.com/en-us/library/windows/desktop/dd183393(v=vs.85).aspx
			//the bitmap must be premultiplied with the alpha channel value
			multiplyRGBWithAlpha(pixels);
			//기존 RGB데이터를 alpha값을 곱한 값으로 교체
			SetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, height, &pixels[0], &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		}

		ReleaseDC(_hWnd, hdc);

		//투명 컬러 세팅
		_transMode = transparentMode;
		_keyColor = keyColor;

		//초기화를 끝내고 Select해주자.
		_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
		return S_OK;
	}

	HRESULT BitmapImage::init(const TCHAR * fileName, TransparentModes transparentMode, COLORREF keyColor)
	{
		//경로값이 없으면
		if (fileName == NULL) return E_FAIL;

		//재할당 방지
		if (_imageInfo != NULL) release();

		HDC hdc = GetDC(BitmapImage::_hWnd);
		_imageInfo = new IMAGE_INFO();
		//비트맵 출력을 위해서 메모리 DC를 만들어 준다.
		_imageInfo->hMemDC = CreateCompatibleDC(hdc);
		//실제 이미지 크기로 이미지를 로드한다.
		_imageInfo->hBit = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		////GEtDIBits함수를 호출할 때 해당 비트맵이 Select되어 있으면 안된다고 한다.
		////https://msdn.microsoft.com/en-us/library/windows/desktop/dd144879(v=vs.85).aspx
		////The bitmap identified by the hbmp parameter must not be selected into a device context when the application calls this function.
		//_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);

		//이미지 초기화에 실패하면
		if (_imageInfo->hBit == NULL) {
			release();
			return E_FAIL;
		}

		//읽어들인 이미지 파일의 실제 크기를 알아낸다.
		BITMAP bmp = { 0 };
		GetObject(_imageInfo->hBit, sizeof(bmp), &bmp);
		_imageInfo->width = bmp.bmWidth;
		_imageInfo->height = bmp.bmHeight;
		_imageInfo->BPP = bmp.bmBitsPixel;

		//BITMAPINFO 구조체 채우기
		GetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, bmp.bmHeight, NULL, &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		_imageInfo->bmpInfo.bmiHeader.biCompression = BI_RGB;//이 멤버를 BI_RGB로 세팅해주지 않으면 GetDIBBits함수 호출 후 뜬금없는 곳에서 에러가 발생한다.

		if (transparentMode == TRANSPARENT_MODE_ALPHA) {
			////픽셀 데이터를 로드한다.
			std::vector<DWORD> pixels(bmp.bmWidth* bmp.bmHeight);
			getPixelData(pixels);

			////로드한 비트맵 파일이 pixel별 알파값을 가지고 있는지 확인한다.
			//int nPixels = bmp.bmWidth*bmp.bmHeight;
			//ARGB* px = (ARGB*)&pixels[0];
			//BYTE sampleAlpha = px->rgb.a; //모든 픽셀이 동일한 알파값을 가지고 있는지 확인
			//_hasAlpha = false;
			//for (int i = 0; i < nPixels; ++i, ++px) {
			//	if (px->rgb.a != sampleAlpha) {
			//		_hasAlpha = true;
			//		break;
			//	}
			//}

			//픽셀별 Alpha값을 가지고 있다면 모든 픽셀의 RGB값에 각각의 alpha값을 곱한다.
			//https://msdn.microsoft.com/en-us/library/windows/desktop/dd183393(v=vs.85).aspx
			//the bitmap must be premultiplied with the alpha channel value
			multiplyRGBWithAlpha(pixels);
			//기존 RGB데이터를 alpha값을 곱한 값으로 교체
			SetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, bmp.bmHeight, &pixels[0], &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
		}

		ReleaseDC(_hWnd, hdc);

		//투명 컬러 세팅
		_transMode = transparentMode;
		_keyColor = keyColor;

		//초기화를 끝내고 Select해주자.
		_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
		return S_OK;
	}

	void BitmapImage::release()
	{
		if (_imageInfo) {
			releaseImage(_imageInfo);
			delete _imageInfo;
			_imageInfo = NULL;
		}

		if (_xMirrored) {
			_xMirrored->release();
			delete _xMirrored;
			_xMirrored = NULL;
		}

		if (_maskImageInfo) {
			releaseImage(_maskImageInfo);
			delete _maskImageInfo;
			_maskImageInfo = NULL;
		}
	}

	void BitmapImage::initXMirroredImage() const
	{
		assert(_xMirrored == NULL);
		_xMirrored = createMirroredImage();
	}

	void BitmapImage::render(HDC hdc, int destX, int destY) const
	{
		render(hdc, destX, destY, getWidth(), getHeight(), 0, 0, getWidth(), getHeight());
	}

	void BitmapImage::render(HDC hdc, int destX, int destY, int destWidth, int destHeight) const
	{
		render(hdc, destX, destY, destWidth, destHeight, 0, 0, getWidth(), getHeight());
	}

	void BitmapImage::render(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight) const
	{
		render(hdc, destX, destY, std::abs(srcWidth), srcHeight, srcX, srcY, srcWidth, srcHeight);
	}

	void BitmapImage::render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight) const
	{
		assert(destWidth >= 0 && destHeight >= 0 && srcHeight>=0);
		if (srcWidth < 0) {
			if (_xMirrored == NULL) {
				initXMirroredImage();
			}
			_xMirrored->render(hdc,
				destX, destY,
				destWidth, destHeight,
				_xMirrored->getWidth() - srcX + srcWidth, srcY,
				-srcWidth, srcHeight);
		}
		else {
			if (_transMode == TRANSPARENT_MODE_NONE) {
				if (srcWidth == destWidth && srcHeight == destHeight) {
					BitBlt(hdc,
						destX, destY,
						destWidth,
						destHeight,
						_imageInfo->hMemDC,
						srcX, srcY,
						SRCCOPY);
				}
				else {
					StretchBlt(hdc,
						destX, destY,
						destWidth, destHeight,
						_imageInfo->hMemDC,
						srcX, srcY,
						srcWidth, srcHeight, SRCCOPY);
				}
			}
			else {
				GdiTransparentBlt(
					hdc,				//복사할 대상이 되는 곳(주로 화면 DC)
					destX,				//복사할 이미지의 좌표 x
					destY,				//복사할 이미지의 좌표 y
					destWidth,			//복사할 이미지의 폭
					destHeight,			//복사할 이미지의 높이
					_imageInfo->hMemDC,	//복사될 대상 이미지의 DC
					srcX,				//복사될 대상 이미지의 복사 시작점 x
					srcY,				//복사될 대상 이미지의 복사 시작점 y
					srcWidth,			//복사될 대상 이미지의 폭
					srcHeight,			//복사될 대상 이미지의 높이
					_keyColor			//투명으로 처리할 색상
				);
			}
		}
	}

	void BitmapImage::renderRot(HDC hdc, int destX, int destY, float angleRad, int pivotX, int pivotY) const
	{
		renderRot(hdc, destX, destY, 0, 0, _imageInfo->width, _imageInfo->height, angleRad, pivotX, pivotY);
	}

	void BitmapImage::renderRot(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, float angleRad, int pivotX, int pivotY) const
	{
		if (srcWidth >= 0) {
			if (_maskImageInfo == NULL && _transMode != TRANSPARENT_MODE_NONE) {
				initMaskImage();
			}
			if (std::abs(angleRad - PI) < 0.1) {
				angleRad = PI - 0.05;
				//TODO 
			}
			RECT destRect = SGA::makeRect<RECT, int>(destX, destY, srcWidth, srcHeight);
			POINT destPoints[4];
			POINT center{ destX + pivotX, destY + pivotY };
			SGA::getRotatedRECT(destRect, angleRad, destPoints, center);
			destPoints[2] = destPoints[3];
			assert(PlgBlt(hdc, destPoints, _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, _maskImageInfo->hBit, 0, 0) != 0);
			//assert(PlgBlt(hdc, destPoints, _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, NULL, 0, 0) != 0);
		}
		else {
			if (_xMirrored == NULL) {
				initXMirroredImage();
			}
			_xMirrored->renderRot(hdc,
				destX, destY,
				_xMirrored->getWidth() - srcX + srcWidth, srcY,
				-srcWidth, srcHeight,
				-angleRad,
				pivotX,
				pivotY);
		}
	}

	void BitmapImage::renderAlpha(HDC hdc, int destX, int destY, BYTE alpha) const
	{
		renderAlpha(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			0, 0, _imageInfo->width, _imageInfo->height, alpha);
	}

	void BitmapImage::renderAlpha(HDC hdc, int destX, int destY, int destWidth, int destHeight, BYTE alpha) const
	{
		renderAlpha(hdc, destX, destY, destWidth, destHeight, 
			0, 0, _imageInfo->width, _imageInfo->height, alpha);
	}

	void BitmapImage::renderAlpha(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) const
	{
		if(srcWidth>0)
			renderAlpha(hdc, destX, destY, srcWidth, srcHeight, 
				srcX, srcY, srcWidth, srcHeight, alpha);
		else 
			renderAlpha(hdc, destX, destY, -srcWidth, srcHeight,
				srcX, srcY, srcWidth, srcHeight, alpha);
	}

	void BitmapImage::renderAlpha(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) const
	{
		if (srcWidth >= 0) {
			BLENDFUNCTION bf;
			if(_transMode == TRANSPARENT_MODE_ALPHA) bf.AlphaFormat = AC_SRC_ALPHA;
			else		bf.AlphaFormat = 0;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;
			bf.SourceConstantAlpha = alpha;
			assert(AlphaBlend(hdc, destX, destY, destWidth, destHeight,
				_imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, bf) != FALSE);
		}
		else {
			assert(destWidth >= 0);
			assert(destHeight >= 0);
			assert(srcHeight >= 0);
			_xMirrored->renderAlpha(hdc,
				destX, destY,
				destWidth, destHeight,
				_xMirrored->getWidth() - srcX + srcWidth, srcY,
				-srcWidth, srcHeight,
				alpha);
		}
	}

	COLORREF BitmapImage::getPixel(int ix, int iy) const {
		return GetPixel(_imageInfo->hMemDC, ix, iy);
	}

	void BitmapImage::setPixel(int ix, int iy, COLORREF color) {
		SetPixel(_imageInfo->hMemDC, ix, iy, color);
	}

	void BitmapImage::getPixelData(std::vector<DWORD>& pixels) const
	{
		if (pixels.size() < getWidth()*getHeight()) {
			pixels.resize(getWidth()*getHeight());
		}
		int ret = GetDIBits(_imageInfo->hMemDC, _imageInfo->hBit, 0, getHeight(), pixels.data(), &(_imageInfo->bmpInfo), DIB_RGB_COLORS);
	}

	void BitmapImage::multiplyRGBWithAlpha(std::vector<DWORD>& pixels)
	{
		ARGB* px = (ARGB*)&pixels[0];
		float s;
		int nPixels = pixels.size();
		for (int i = 0; i < nPixels; ++i, ++px) {
			s = (px->rgb.a) / 255.0f;
			px->rgb.r = static_cast<BYTE>(px->rgb.r*s);
			px->rgb.g = static_cast<BYTE>(px->rgb.g*s);
			px->rgb.b = static_cast<BYTE>(px->rgb.b*s);
		}
	}

	void BitmapImage::clearImage()
	{
		clearImage(_keyColor);
	}

	void BitmapImage::clearImage(COLORREF color)
	{
		SelectObject(_imageInfo->hMemDC, GetStockObject(DC_PEN));
		COLORREF colorOldPen = SetDCPenColor(_imageInfo->hMemDC, color);
		SelectObject(_imageInfo->hMemDC, GetStockObject(DC_BRUSH));
		COLORREF colorOld = SetDCBrushColor(_imageInfo->hMemDC, color);
		Rectangle(_imageInfo->hMemDC, 0, 0, getWidth(), getHeight());
		SetDCBrushColor(_imageInfo->hMemDC, colorOld);
		SetDCPenColor(_imageInfo->hMemDC, colorOldPen);
	}

	void BitmapImage::initMaskImage() const
	{
		assert(_maskImageInfo == NULL);

		int width = getWidth(), height = getHeight();
		int nBytes = width;
		nBytes = nBytes / 8 + (nBytes % 8 ? 1 : 0);
		int nWord = nBytes / 2 + (nBytes % 2 ? 1 : 0);
		nBytes = 2 * nWord;

	
		std::vector<BYTE> maskData(nBytes*height, 0);
		int cnt = 0;
		int cntByte = 0;
		//int offset = 13;
		int offset = 0;

		int pixelCount = 0;
		std::vector<DWORD> pixels(width*height);
		getPixelData(pixels);
		for (int iy = 0; iy < height; ++iy) {
			cnt = offset;
			pixelCount = width *(height - iy - 1);
			for (int ix = 0; ix < width; ++ix, ++cnt,++pixelCount) {
				if (cnt == 8) {
					cnt = 0;
					cntByte++;
				}
				if ((pixels[pixelCount] & 0x00ffffff) != _keyColor) {
					maskData[cntByte] |= (0x80 >> cnt);
				}
			}
			cntByte++;
			if (cntByte % 2) {
				cntByte++;
			}
		}
		_maskImageInfo = new IMAGE_INFO;
		//비트맵 출력을 위해서 메모리 DC를 만들어 준다
		_maskImageInfo->hMemDC = NULL;
		//_maskImageInfo->hBit = (HBITMAP)CreateBitmap(getWidth(), getHeight(), 1, 1, &maskData[0]);
		_maskImageInfo->hBit = (HBITMAP)CreateBitmap(width, height, 1, 1, &maskData[0]);

		_maskImageInfo->hOBit = NULL;
		_maskImageInfo->width = width;
		_maskImageInfo->height = height;
		_maskImageInfo->BPP = 8;
	}

	void BitmapImage::releaseImage(LPIMAGE_INFO img)
	{
		if (img) {
			SelectObject(img->hMemDC, img->hOBit);
			DeleteObject(img->hBit);
			if (img->hMemDC)
				DeleteDC(img->hMemDC);
		}
	}

}
/*

*/