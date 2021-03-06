#include "ColorPalette.h"

using namespace ofxColorPalette;

ColorPalette::ColorPalette(PALETTE_TYPE palette, float rootHue) {
	resetPalette(palette, rootHue);
}

ColorPalette::ColorPalette(PALETTE_TYPE palette) {
	resetPalette(palette, ofRandom(255.0));
}


ColorPalette::~ColorPalette() {

}

std::shared_ptr<ofColor> ColorPalette::nextColor() {
	float hue = NULL;

	if (mHueType == UNIFORM || mPaletteType == MONOCHROME) {
		// a bit of an odd special case -- MONOCHROME palettes only have one color, so we treat it as a uniform distribution.
		hue = std::fmod(mHues[(int)ofRandom(mHues.size())], 255.0);
	}
	else {
		float p = ofRandom(1.0);
		if (p < mRootProbability) {
			hue = mHues[0];
		}
		else {
			hue = std::fmod(mHues[(int)ofRandom(mHues.size() - 1) + 1], 255.0);
		}
	}
	float saturation;
	float brightness;
	float randomSeed = ofRandom(1.0);
	if (randomSeed < mBlackProbability) {
		// set to black
		saturation = 0.0;
		brightness = 0.0;
	}
	else if (randomSeed < mBlackProbability + mWhiteProbability) {
		// set to white
		saturation = 0.0;
		brightness = mMaximumBrightness;
	}
	else {
		// if not white or black, then pick a hued color based off of saturation scheme:
		switch(mSaturationType) {
			case STATIC:
				saturation = mMaximumSaturation;
				brightness = mMaximumBrightness;
				break;
			case LEVELS:
				saturation = mSaturationLevels[(int)ofRandom(mSaturationLevels.size())];
				brightness = mMaximumBrightness;
				break;
			case RANDOM:
				saturation = ofRandom(mMinimumSaturation, mMaximumSaturation);
				brightness = mMaximumBrightness;
				break;
			default:
				saturation = mMaximumSaturation;
				brightness = mMaximumBrightness;
				break;
		}
	}

	std::shared_ptr<ofColor> nextColor = std::shared_ptr<ofColor>(new ofColor(0));
	nextColor->setHsb(hue, saturation, brightness);
	return nextColor;
}

float ColorPalette::getRootHue() {
	return mRootHue;
}

void ColorPalette::setRootHue(float root) {
	mRootHue = root;

	mHues.clear();
	mHues.push_back(mRootHue);

	switch (mPaletteType) {
	case MONOCHROME:
		break;
	case COMPLEMENTARY:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 2.0, 255));
		break;
	case SPLIT_COMPLEMENTARY:
		mHues.push_back(std::fmod(mRootHue + 255.0 * 5.0 / 12.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 7.0 / 12.0, 255.0));
		break;
	case TRIADIC:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 3.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 2.0 / 3.0, 255.0));
		break;
	case TETRADIC:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 4.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 2.0 / 4.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 3.0 / 4.0, 255.0));
		break;
	case FOUR_TONE:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 6.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 3.0 / 6.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 4.0 / 6.0, 255.0));
		break;
	case FIVE_TONE:
		mHues.push_back(std::fmod(mRootHue + 255.0 * 23.0 / 72.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 31.0 / 72.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 41.0 / 72.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 49.0 / 72.0, 255.0));
		break;
	case SIX_TONE:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 12.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 4.0 / 12.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 5.0 / 12.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 8.0 / 12.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 9.0 / 12.0, 255.0));
		break;
	case NEUTRAL:
		mHues.push_back(std::fmod(mRootHue + 255.0 / 24.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 2.0 / 24.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 3.0 / 24.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 4.0 / 24.0, 255.0));
		mHues.push_back(std::fmod(mRootHue + 255.0 * 5.0 / 24.0, 255.0));
		break;
	}
}

std::vector<float> ColorPalette::getHues() {
	return mHues;
}

void ColorPalette::init(PALETTE_TYPE palette) {
	mPaletteType = palette;
	mHueType = UNIFORM;
	mSaturationType = LEVELS;
	setNumberOfSaturationLevels(8);
	mBlackProbability = 0.0;
	mWhiteProbability = 0.0;
	mSaturatedProbability = 0.0;
	mRootProbability = 0.0;
	randomizePalette();
	mMaximumBrightness = 255.0;
	mMaximumSaturation = 255.0;
	mMinimumBrightness = 0.0;
	mMinimumSaturation = 0.0;

	setRootHue(mRootHue);
}

void ColorPalette::resetPalette(PALETTE_TYPE palette, float rootHue) {
	mRootHue = rootHue;
	init(palette);
}

void ColorPalette::setPaletteType(PALETTE_TYPE type) {
	mPaletteType = type;
	setRootHue(mRootHue);
}

PALETTE_TYPE ColorPalette::getPaletteType() {
	return mPaletteType;
}


void ColorPalette::randomizePalette() {
	float randomSeed = ofRandom(1.0);
	if (randomSeed < 0.3) {
		mSaturatedProbability = 1.0;
		mBlackProbability = 0;
		mWhiteProbability = 0;
	}
	else if (randomSeed < 0.4) {
		mBlackProbability = 1.0 / (1 + mHues.size());
		mSaturatedProbability = 1.0 - mBlackProbability;
		mWhiteProbability = 0.0;
	}
	else if (randomSeed < 0.5) {
		mWhiteProbability = 1.0 / (1 + mHues.size());
		mSaturatedProbability = 1.0 - mWhiteProbability;
		if (ofRandom(1.0) < 0.5) {
			setHueType(SKEWED_ROOT);
			mRootProbability = ofRandom(1.0);
		}
		else {
			setHueType(UNIFORM);
		}
		mBlackProbability = 0.0;
	}
	else if (randomSeed < 0.6) {
		mBlackProbability = 1.0 / (2 + mHues.size());
		mWhiteProbability = 1.0 / (2 + mHues.size());
		mSaturatedProbability = 1 - mBlackProbability - mWhiteProbability;
		if (ofRandom(1.0) < 0.5) {
			setHueType(SKEWED_ROOT);
			mRootProbability = ofRandom(1.0);
		}
		else {
			setHueType(UNIFORM);
		}
	}
}

void ColorPalette::setProbabilities(float black, float white, float saturated) {
	if (black + white + saturated != 1.0) {
		std::printf("ColorPalette::setProbabilities -- WARNING Probabilities do not add up to one!\n");
	}
	if (mHueType != UNIFORM) {
		mHueType = UNIFORM;
		std::printf("ColorPalette::setProbabilities -- Hue distribution type set to UNIFORM.\n");
	}
	mBlackProbability = black;
	mWhiteProbability = white;
	mSaturatedProbability = saturated;
}

void ColorPalette::setProbabilities(float black, float white, float saturated, float root) {
	if (black + white + saturated != 1.0) {
		std::printf("ColorPalette::setProbabilities -- WARNING Probabilities do not add up to one!\n");
	}
	if (mHueType != SKEWED_ROOT) {
		mHueType = SKEWED_ROOT;
		std::printf("ColorPalette::setProbabilities -- Hue distribution type set to SKEWED_ROOT.\n");
	}
	mBlackProbability = black;
	mWhiteProbability = white;
	mSaturatedProbability = saturated;
	mRootProbability = root;
}

void ColorPalette::setHueType(HUE_TYPE type) {
	mHueType = type;
}

HUE_TYPE ColorPalette::getHueType() {
	return mHueType;
}

float ColorPalette::getBlackProbability() {
	return mBlackProbability;
}

float ColorPalette::getWhiteProbability() {
	return mWhiteProbability;
}

float ColorPalette::getSaturatedProbability() {
	return mSaturatedProbability;
}

void ColorPalette::setSaturationType(SATURATION_TYPE type) {
	mSaturationType = type;
}

SATURATION_TYPE ColorPalette::getSaturationType() {
	return mSaturationType;
}

void ColorPalette::setMaximumSaturation(float maximum_saturation) {
	mMaximumSaturation = maximum_saturation;
}

float ColorPalette::getMaximumSaturation() {
	return mMaximumSaturation;
}

void ColorPalette::setMinimumSaturation(float minimum_saturation) {
	mMinimumSaturation = minimum_saturation;
}

float ColorPalette::getMinimumSaturation() {
	return mMinimumSaturation;
}

void ColorPalette::setNumberOfSaturationLevels(int levels) {
	mSaturationLevels.clear();
	if (mSaturationType != LEVELS) {
		mSaturationType = LEVELS;
	}
	for (int i = 0; i < levels; i++) {
		//saturation levels do NOT include pure white -- set a white probability if you want white!
		mSaturationLevels.push_back(mMaximumSaturation * (i + 1) / (levels + 1));
	}
}

int ColorPalette::getNumberOfSaturationLevels() {
	return mSaturationLevels.size();
}

std::vector<float> ColorPalette::getSaturationLevels() {
	return mSaturationLevels;
}

void ColorPalette::setMaximumBrightness(float maximum_brightness) {
	mMaximumBrightness = maximum_brightness;
}

float ColorPalette::getMaximumBrightness() {
	return mMaximumBrightness;
}

void ColorPalette::setMinimumBrightness(float minimum_brightness) {
	mMinimumBrightness = minimum_brightness;
}

float ColorPalette::getMinimumBrightness() {
	return mMinimumBrightness;
}