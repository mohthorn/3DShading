//
//  ColorData.h
//
//  Created by Shenyao Ke on 10/11/14.
//  Copyright (c) 2014 Shenyao Ke. All rights reserved.
//
#pragma once
#ifndef __ColorData__
#define __ColorData__

#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;

//Class declaration
class ColorWebRGB;
class ColorRGB;
class ColorRGBA;
class ColorHSV;
class ColorHSVA;

template <typename T>
inline const T& clamp(const T& data, const T& minValue, const T& maxValue)
{
	return data > maxValue ? maxValue : (data < minValue ? minValue : data);
}
template <typename T2>
inline const T2& clampFromZeroToOne(const T2& data)
{
	return clamp(data, static_cast<T2>(0), static_cast<T2>(1));
}

class ColorWebRGB
{
public:
	unsigned char r, g, b;
	ColorWebRGB();
	ColorWebRGB(unsigned char red, unsigned char green, unsigned char blue);
	~ColorWebRGB();

private:

};

inline ColorWebRGB::ColorWebRGB()
{
	r = 0;
	g = 0;
	b = 0;
}
inline ColorWebRGB::ColorWebRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	r = red; g = green; b = blue;
}
inline ColorWebRGB::~ColorWebRGB()
{
}
// RGB color space
class ColorRGB
{
private:
protected:
public:
    double r, g, b; // RGB 0-1.0
    
	ColorRGB() : r(0), g(0), b(0){}
	ColorRGB(int red, int green, int blue){ r = static_cast<double>(red) / 255.0; g = static_cast<double>(green) / 255.0; b = static_cast<double>(blue) / 255.0; }
	ColorRGB(double red, double green, double blue) : r(red), g(green), b(blue){}
    
	virtual ~ColorRGB(){};
    //ColorRGB(double hue, double saturation, double value);
	ColorRGB operator+(const ColorRGB& color2) const { return ColorRGB(r + color2.r, g + color2.g, b + color2.b); }
	ColorRGB operator-(const ColorRGB& color2) const { return ColorRGB(r - color2.r, g - color2.g, b - color2.b); }
	ColorRGB operator*(const ColorRGB& color2) const { return ColorRGB(r * color2.r, g * color2.g, b * color2.b); }
	ColorRGB operator/(const ColorRGB& color2) const { return ColorRGB(r / color2.r, g / color2.g, b / color2.b); }
	ColorRGB operator*(double n) const { return ColorRGB(r * n, g * n, b * n); }
	ColorRGB operator/(double n) const { return ColorRGB(r / n, g / n, b / n); }
	
	ColorRGB& operator+=(const ColorRGB& color2) { *this = *this + color2; return *this; }
	ColorRGB& operator-=(const ColorRGB& color2) { *this = *this - color2; return *this; }
	ColorRGB& operator*=(double n) { *this = *this * n; return *this; }
	ColorRGB& operator/=(double n) { *this = *this / n; return *this; }

	friend ColorRGB operator*(double n, const ColorRGB& color){ return color * n; }
	friend ostream& operator<<(ostream &os, const ColorRGB &color);

	virtual void setRGB(double red, double green, double blue){ r = red; g = green; b = blue; };
	virtual void setWebColor(unsigned char red, unsigned char green, unsigned char blue){ r = red / 255.0; g = green / 255.0; b = blue / 255.0; };
	virtual double getLuma(){ return 0.2126 * r + 0.7152 * g + 0.0722 * b; }
	virtual void printInfo() const{ std::cout << "Color data\n\tRed:\t" << this->r << "\n\tGreen:\t" << this->g << "\n\tBlue:\t" << this->b << std::endl; }
    
	const ColorHSV& conv2hsv() const;
	void clamp();
	ColorRGB returnClamp();

};
class ColorRGBA :public ColorRGB
{
private:
public:
	double a;//alpha channel
	ColorRGBA() : ColorRGB(), a(1){}
	ColorRGBA(int red, int green, int blue) : ColorRGB(red, green, blue), a(1){}
	ColorRGBA(double red, double green, double blue) : ColorRGB(red, green, blue), a(1){}
	ColorRGBA(double red, double green, double blue, double alpha) : ColorRGB(red, green, blue), a(alpha){}
	ColorRGBA(ColorRGB rgb) : ColorRGB(rgb.r, rgb.g, rgb.b), a(1){}
	ColorRGBA(ColorRGB rgb, double alpha) : ColorRGB(rgb.r, rgb.g, rgb.b), a(alpha){}
	~ColorRGBA(){}

	ColorRGBA operator+(const ColorRGBA& color2) const { return ColorRGBA(r + color2.r, g + color2.g, b + color2.b, a + color2.a); }
	ColorRGBA operator-(const ColorRGBA& color2) const { return ColorRGBA(r - color2.r, g - color2.g, b - color2.b, a - color2.a); }
	ColorRGBA operator*(const ColorRGBA& color2) const { return ColorRGBA(r * color2.r, g * color2.g, b * color2.b, a * color2.a); }
	ColorRGBA operator/(const ColorRGBA& color2) const { return ColorRGBA(r / color2.r, g / color2.g, b / color2.b, a / color2.a); }
	ColorRGBA operator*(double n) const { return ColorRGBA(r * n, g * n, b * n, a * n); }
	ColorRGBA operator/(double n) const { return ColorRGBA(r / n, g / n, b / n, a / n); }

	ColorRGBA& operator+=(const ColorRGBA& color2) { *this = *this + color2; return *this; }
	ColorRGBA& operator-=(const ColorRGBA& color2) { *this = *this - color2; return *this; }
	ColorRGBA& operator*=(double n) { *this = *this * n; return *this; }
	ColorRGBA& operator/=(double n) { *this = *this / n; return *this; }
	ColorRGBA& operator=(const ColorRGB& color2) { this->r = color2.r; this->g = color2.g; this->b = color2.b; return *this; }
	friend ColorRGBA operator*(double n, const ColorRGBA& color1){ return color1 * n; }
	friend ostream& operator<<(ostream &os, const ColorRGBA &color);

	ColorRGBA compAdd(const ColorRGBA& color2) const{ return *this * this->a + color2 * (1 - this->a); }
	friend ColorRGBA compAdd(const ColorRGBA& color1, const ColorRGBA& color2);

	ColorRGBA multiplyRGB(double n) const{ return ColorRGBA(r * n, g * n, b * n, a); }

	void setRGBA(double red, double green, double blue, double alpha){ setRGB(red, green, blue); a = alpha; };
	void printInfo() const{ std::cout << "Color data\n\tRed:\t" << this->r << "\n\tGreen:\t" << this->g << "\n\tBlue:\t" << this->b << "\n\tAlpha:\t" << this->a << std::endl; };

	const ColorHSVA& conv2hsva() const;

	void clamp();
	ColorRGBA returnClamp();
};
inline ColorRGBA compAdd(const ColorRGBA& color1, const ColorRGBA& color2)
{
	ColorRGBA ret = color1 * color1.a + color2 * (1 - color1.a);
	//
	return ret;
}
// HSV color space
class ColorHSV
{
public:
	double h, s, v;//H is from 0-360
	ColorHSV();
	ColorHSV(double hue, double saturation, double value){ h = hue; s = saturation; v = value; }

	const ColorRGB& conv2rgb() const;

	void clamp();
};


inline const ColorRGB& ColorHSV::conv2rgb() const
{
	double r, g, b;
	double h0 = h / 60.0;

	double chroma = s * v;
	double x = chroma * (1.0 - abs(fmod(h0, 2.0) - 1.0));

	double min = v - chroma;

	if (h0 == 0)
	{
		r = min;
		g = min;
		b = min;
	}
	else if (h0 < 1.0)
	{
		r = (min + chroma);
		g = (min + x);
		b = min;
	}
	else if (h0 < 2.0)
	{
		r = (min + x);
		g = (min + chroma);
		b = min;
	}
	else if (h0 < 3.0)
	{
		r = min;
		g = (min + chroma);
		b = (min + x);
	}
	else if (h0 < 4.0)
	{
		r = min;
		g = (min + x);
		b = (min + chroma);
	}
	else if (h0 < 5.0)
	{
		r = (min + x);
		g = min;
		b = (min + chroma);
	}
	else if (h0 < 6.0)
	{
		r = (min + chroma);
		g = min;
		b = (min + x);
	}
	return ColorRGB(r, g, b);
}

inline void ColorHSV::clamp()
{
	while (h < 0.0)
	{
		h += 360.0;
	}
	while (h >= 360.0)
	{
		h -= 360.0;
	}
	clampFromZeroToOne(s);
	clampFromZeroToOne(v);
}

inline void ColorRGB::clamp()
{
	r = r > 1 ? 1 : (r < 0 ? 0 : r);
	g = g > 1 ? 1 : (g < 0 ? 0 : g);
	b = b > 1 ? 1 : (b < 0 ? 0 : b);
}
inline ColorRGB ColorRGB::returnClamp()
{
	double retR = r > 1 ? 1 : (r < 0 ? 0 : r);
	double retG = g > 1 ? 1 : (g < 0 ? 0 : g);
	double retB = b > 1 ? 1 : (b < 0 ? 0 : b);
	return ColorRGB(retR, retG, retB);
}
inline void ColorRGBA::clamp()
{
	ColorRGB::clamp();
	a = a > 1 ? 1 : (a < 0 ? 0 : a);
}

inline ColorRGBA ColorRGBA::returnClamp()
{
	double retR = r > 1 ? 1 : (r < 0 ? 0 : r);
	double retG = g > 1 ? 1 : (g < 0 ? 0 : g);
	double retB = b > 1 ? 1 : (b < 0 ? 0 : b);
	double retA = a > 1 ? 1 : (a < 0 ? 0 : a);
	ColorRGBA ret(retR, retG, retB, retA);
	return ret;
}


//Convert RGB color data to HSV
inline const ColorHSV& ColorRGB::conv2hsv() const
{
	double h, s, v;
	double max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
	double min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

	v = max;
	double chroma = max - min;
	if (max == 0)
	{
		h = 0.0;
		s = 0.0;
	}
	else
	{
		s = chroma / max;

		if (chroma == 0)
		{
			h = 0.0;
		} 
		else if (max == r)
		{
			h = 60 * fmod(static_cast<double>(g - b) / chroma, 6);
		}
		else if (max == g)
		{
			h = 60 * ((b - r) / chroma + 2);
		}
		else if (max == b)
		{
			h = 60 * ((r - g) / chroma + 4);
		}
		
		h = (h >= 0) ? h : (h + 360.0); //
	}
	return ColorHSV(h, s, v);
}

inline ostream& operator<<(ostream &os, const ColorRGB &color)
{
	os << "Color data\n\tRed:\t" << color.r << "\n\tGreen:\t" << color.g << "\n\tBlue:\t" << color.b;
	return os;
}
inline ostream& operator<<(ostream &os, const ColorRGBA &color)
{
	os << "Color data\n\tRed:\t" << color.r << "\n\tGreen:\t" << color.g << "\n\tBlue:\t" << color.b << "\n\tAlpha:\t" << color.a;
	return os;
}
//Color Preset
// From https://pythonhosted.org/ete2/reference/reference_svgcolors.html
//Red Colors
static const ColorRGBA COLOR_LIGHT_CORAL(0xF0, 0x80, 0x80);
static const ColorRGBA COLOR_SALMON(0xFA, 0x80, 0x72);
static const ColorRGBA COLOR_CRIMSON(0xDC, 0x14, 0x3C);
static const ColorRGBA COLOR_RED(0xFF, 0x00, 0x00);
//Pink Colors
static const ColorRGBA COLOR_PINK(0xFF, 0xC0, 0xCB);
static const ColorRGBA COLOR_DEEP_PINK(0xFF, 0x14, 0x93);
//Orange Colors
static const ColorRGBA COLOR_CORAL(0xFF, 0x7F, 0x50);
static const ColorRGBA COLOR_TOMATO(0xFF, 0x45, 0x00);
static const ColorRGBA COLOR_ORANGE_RED(0xFF, 0x45, 0x00);
static const ColorRGBA COLOR_ORANGE(0xFF, 0xA5, 0x00);
//Yellow Colors
static const ColorRGBA COLOR_GOLD(0xFF, 0xD7, 0x00);
static const ColorRGBA COLOR_YELLOW(0xFF, 0xFF, 0x00);
static const ColorRGBA COLOR_LIGHT_YELLOW(0xFF, 0xFF, 0xE0);
static const ColorRGBA COLOR_LEMON_CHIFFON(0xFF, 0xFA, 0xCD);
//Purple Colors
static const ColorRGBA COLOR_LAVENDER(0xE6, 0xE6, 0xFA);
static const ColorRGBA COLOR_VIOLET(0xEE, 0x82, 0xEE);
static const ColorRGBA COLOR_PURPLE(0x80, 0x00, 0x80);
static const ColorRGBA COLOR_INDIGO(0x4B, 0x00, 0x82);
static const ColorRGBA COLOR_SLATE_BLUE(0x6A, 0x5A, 0xCD);
static const ColorRGBA COLOR_GREEN(0x00, 0xFF, 0x00);
static const ColorRGBA COLOR_BLUE(0x00, 0x00, 0xFF);
static const ColorRGBA COLOR_WHITE(0xFF, 0xFF, 0xFF);
static const ColorRGBA COLOR_BLACK(0x00, 0x00, 0x00);
//Blue Colors
static const ColorRGBA COLOR_SKY_BLUE(0x87, 0xCE, 0xEB);
static const ColorRGBA COLOR_DEEP_SKY_BLUE(0x00, 0xBF, 0xFF);
//Brown Color
static const ColorRGBA COLOR_SADDLE_BROWN(0x8B, 0x45, 0x13);

//
static const ColorRGBA COLOR_TRANSPARENT_BLACK(0, 0, 0, 0);
#endif /* defined(__ColorData__) */