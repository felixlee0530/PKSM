/*  This file is part of PKSM
>	Copyright (C) 2016/2017 Bernardo Giordano
>
>   This program is free software: you can redistribute it and/or modify
>   it under the terms of the GNU General Public License as published by
>   the Free Software Foundation, either version 3 of the License, or
>   (at your option) any later version.
>
>   This program is distributed in the hope that it will be useful,
>   but WITHOUT ANY WARRANTY; without even the implied warranty of
>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>   GNU General Public License for more details.
>
>   You should have received a copy of the GNU General Public License
>   along with this program.  If not, see <http://www.gnu.org/licenses/>.
>   See LICENSE for information.
*/

#include "graphic.h"

u32 logoColors[15] = {RGBA8(0, 136, 222, 255), 
					RGBA8(203, 39, 69, 255),
					RGBA8(247, 139, 50, 255),
					RGBA8(115, 118, 252, 255),
					RGBA8(241, 148, 45, 255),
					RGBA8(75, 139, 190, 255),
					RGBA8(138, 207, 231, 255),
					RGBA8(195, 135, 182, 255),
					RGBA8(196, 170, 76, 255),
					RGBA8(248, 169, 64, 255),
					RGBA8(108, 131, 185, 255),
					RGBA8(227, 236, 243, 255),
					RGBA8(62, 74, 72, 255),
					RGBA8(0, 167, 215, 255),
					RGBA8(223, 105, 95, 255),
};

int logo_lookup6[7] = {0, 64, 121, 183, 250, 320, 375};
int logo_lookup4[6] = {0, 54, 116, 177, 233, 291};
int logo_lookup5[5] = {0, 53, 104, 176, 252};

int lookup[] = {0x0, 0x1, 0x2, 0x4, 0x5, 0x3};

sftd_font *unicodeJPN12, *unicodeKOR12, *fontBold18, *fontBold15, *fontBold14, *fontBold12, *fontBold11, *fontBold9, *fontFixed; 
sf2d_texture *noMove, *hexIcon, *hexBG, *blueTextBox, *otaButton, *generationBG, *includeInfoButton, *hiddenPowerBG, *ballsBG, *male, *female, *naturestx, *movesBottom, *topMovesBG, *editorBar, *editorStatsBG, *subArrow, *backgroundTop, *miniBox, *plusButton, *minusButton, *balls, *typesSheet, *transferButton, *bankTop, *shinyStar, *normalBar, *LButton, *RButton, *creditsTop, *pokeball, *gameSelectorBottom1, *gameSelectorBottom2, *gameSelectorTop, *menuBar, *menuSelectedBar, *darkButton, *eventTop, *left, *lightButton, *redButton, *right, *spritesSmall, *eventMenuBottomBar, *eventMenuTopBarSelected, *eventMenuTopBar, *warningTop, *warningBottom, *boxView, *infoView, *selector, *editorBG, *plus, *minus, *back, *setting, *selectorCloning, *button, *bottomPopUp, *pokemonBufferBox, *DSBottomBG, *DSTopBG, *DSBarSelected, *DSBar, *DSEventBottom, *DSLangSelected, *DSLang, *DSEventTop, *DSNormalBarL, *DSNormalBarR, *DSSelectedBarL, *DSSelectedBarR, *settings, *item, *alternativeSpritesSmall;

char *gamesList[] = {"X", "Y", "Omega Ruby", "Alpha Sapphire", "Sun", "Moon", "Diamond", "Pearl", "Platinum", "Heart Gold", "Soul Silver", "Black", "White", "Black 2", "White 2"};

wchar_t **abilities;
wchar_t **moves;
wchar_t **movesSorted;
wchar_t **natures;

wchar_t **items;
wchar_t **itemsSorted;
wchar_t **hpList;

// char *hpList[] = {"Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel", "Fire", "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", " ", " "};

void GUIElementsInit() {
	fontBold15 = sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold12 = sftd_load_font_file("romfs:/res/Bold.ttf");
	warningTop = sfil_load_PNG_file("romfs:/res/Warning Top.png", SF2D_PLACE_RAM);
	warningBottom = sfil_load_PNG_file("romfs:/res/Warning Bottom.png", SF2D_PLACE_RAM);
	freezeMsg("Loading files...");

	unicodeJPN12 = sftd_load_font_file("romfs:/res/UnicodeJPN.otf");
	unicodeKOR12 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");
	fontBold18 = sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold14 = sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold11 = sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold9 = sftd_load_font_file("romfs:/res/Bold.ttf");
	fontFixed = sftd_load_font_file("romfs:/res/VeraMono.ttf");
	
	freezeMsg("Loading locales assets...");
	struct i18n_files listFiles = i18n_getFilesPath();
	freezeMsgDetails("Loading abilities");
	struct ArrayUTF32 listAbilities = i18n_FileToArrayUTF32(listFiles.abilities);
	freezeMsgDetails("Loading moves");
	listMoves = i18n_FileToArrayUTF32(listFiles.moves);
	freezeMsgDetails("Sorting moves");
	ArrayUTF32_sort_starting_index(&listMoves, 1);
	freezeMsgDetails("Loading natures");
	struct ArrayUTF32 listNatures = i18n_FileToArrayUTF32(listFiles.natures);
	natures = listNatures.items;
	freezeMsgDetails("Loading items");
	listItems = i18n_FileToArrayUTF32(listFiles.items);
	freezeMsgDetails("Sorting items");
	ArrayUTF32_sort_starting_index(&listItems, 1);
	freezeMsgDetails("Loading Hidden powers");
	struct ArrayUTF32 listHPs = i18n_FileToArrayUTF32(listFiles.hp);

	abilities = listAbilities.items;
	moves = listMoves.items;
	movesSorted = listMoves.sortedItems;
	natures = listNatures.items;
	items = listItems.items;
	itemsSorted = listItems.sortedItems;
	hpList = listHPs.items;

	freezeMsg("Loading fonts...");
	init_font_cache();
	freezeMsg("Done!");
}

void GUIElementsSpecify(int game) {
	freezeMsg("Loading graphics...");
	alternativeSpritesSmall = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/alternative_icons_spritesheetv3.png", SF2D_PLACE_RAM);
	spritesSmall = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/pokemon_icons_spritesheetv3.png", SF2D_PLACE_RAM);
	balls = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/balls_spritesheetv2.png", SF2D_PLACE_RAM);
	settings = sfil_load_PNG_file("romfs:/res/Settings.png", SF2D_PLACE_RAM);
	
	if (game < 6) {
		typesSheet = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/types_sheetv2.png", SF2D_PLACE_RAM);
		
		boxView = sfil_load_PNG_file("romfs:/res/Box View.png", SF2D_PLACE_RAM);
		noMove = sfil_load_PNG_file("romfs:/res/No Move.png", SF2D_PLACE_RAM);
		back = sfil_load_PNG_file("romfs:/res/Back Button.png", SF2D_PLACE_RAM);
		editorBar = sfil_load_PNG_file("romfs:/res/Bottom Bar.png", SF2D_PLACE_RAM);
		hexIcon = sfil_load_PNG_file("romfs:/res/Hex Button.png", SF2D_PLACE_RAM);
		hexBG = sfil_load_PNG_file("romfs:/res/Hex BG.png", SF2D_PLACE_RAM);
		blueTextBox = sfil_load_PNG_file("romfs:/res/Blue Textbox.png", SF2D_PLACE_RAM);
		otaButton = sfil_load_PNG_file("romfs:/res/OTA Button.png", SF2D_PLACE_RAM);
		includeInfoButton = sfil_load_PNG_file("romfs:/res/Include Info.png", SF2D_PLACE_RAM);
		generationBG = sfil_load_PNG_file("romfs:/res/Generation BG.png", SF2D_PLACE_RAM);
		hiddenPowerBG = sfil_load_PNG_file("romfs:/res/Hidden Power BG.png", SF2D_PLACE_RAM);
		ballsBG = sfil_load_PNG_file("romfs:/res/BallsBG.png", SF2D_PLACE_RAM);
		male = sfil_load_PNG_file("romfs:/res/Male.png", SF2D_PLACE_RAM);
		female = sfil_load_PNG_file("romfs:/res/Female.png", SF2D_PLACE_RAM);
		naturestx = sfil_load_PNG_file("romfs:/res/Natures.png", SF2D_PLACE_RAM);
		movesBottom = sfil_load_PNG_file("romfs:/res/Moves Bottom.png", SF2D_PLACE_RAM);
		topMovesBG = sfil_load_PNG_file("romfs:/res/Top Moves.png", SF2D_PLACE_RAM);
		editorStatsBG = sfil_load_PNG_file("romfs:/res/Editor Stats.png", SF2D_PLACE_RAM);
		subArrow = sfil_load_PNG_file("romfs:/res/Sub Arrow.png", SF2D_PLACE_RAM);
		backgroundTop = sfil_load_PNG_file("romfs:/res/Background.png", SF2D_PLACE_RAM);
		miniBox = sfil_load_PNG_file("romfs:/res/Mini Box.png", SF2D_PLACE_RAM);
		minusButton = sfil_load_PNG_file("romfs:/res/Minus Button.png", SF2D_PLACE_RAM);
		plusButton = sfil_load_PNG_file("romfs:/res/Plus Button.png", SF2D_PLACE_RAM);
		transferButton = sfil_load_PNG_file("romfs:/res/Transfer Button.png", SF2D_PLACE_RAM);
		bankTop = sfil_load_PNG_file("romfs:/res/Bank Top.png", SF2D_PLACE_RAM);
		shinyStar = sfil_load_PNG_file("romfs:/res/Shiny.png", SF2D_PLACE_RAM);
		normalBar = sfil_load_PNG_file("romfs:/res/Normal Bar.png", SF2D_PLACE_RAM);
		RButton = sfil_load_PNG_file("romfs:/res/R Button.png", SF2D_PLACE_RAM);
		LButton = sfil_load_PNG_file("romfs:/res/L Button.png", SF2D_PLACE_RAM);
		creditsTop = sfil_load_PNG_file("romfs:/res/Credits Top.png", SF2D_PLACE_RAM);
		pokeball = sfil_load_PNG_file("romfs:/res/Pokeball.png", SF2D_PLACE_RAM);
		menuBar = sfil_load_PNG_file("romfs:/res/Main Menu Dark Bar.png", SF2D_PLACE_RAM);
		menuSelectedBar = sfil_load_PNG_file("romfs:/res/Main Menu Red Bar.png", SF2D_PLACE_RAM);
		darkButton = sfil_load_PNG_file("romfs:/res/Dark Button.png", SF2D_PLACE_RAM);
		eventTop = sfil_load_PNG_file("romfs:/res/Event Top.png", SF2D_PLACE_RAM);
		left = sfil_load_PNG_file("romfs:/res/Left.png", SF2D_PLACE_RAM);
		lightButton = sfil_load_PNG_file("romfs:/res/Light Button.png", SF2D_PLACE_RAM);
		redButton = sfil_load_PNG_file("romfs:/res/Red Button.png", SF2D_PLACE_RAM);
		right = sfil_load_PNG_file("romfs:/res/Right.png", SF2D_PLACE_RAM);
		eventMenuBottomBar = sfil_load_PNG_file("romfs:/res/Event Menu Bottom Bar.png", SF2D_PLACE_RAM);
		eventMenuTopBarSelected = sfil_load_PNG_file("romfs:/res/Event Menu Top Bar Selected.png", SF2D_PLACE_RAM);
		eventMenuTopBar = sfil_load_PNG_file("romfs:/res/Event Menu Top Bar.png", SF2D_PLACE_RAM);
		infoView = sfil_load_PNG_file("romfs:/res/Info View.png", SF2D_PLACE_RAM);
		selector = sfil_load_PNG_file("romfs:/res/Selector.png", SF2D_PLACE_RAM);
		selectorCloning = sfil_load_PNG_file("romfs:/res/Selector (cloning).png", SF2D_PLACE_RAM);
		editorBG = sfil_load_PNG_file("romfs:/res/Editor Bottom BG.png", SF2D_PLACE_RAM);
		plus = sfil_load_PNG_file("romfs:/res/Plus.png", SF2D_PLACE_RAM);
		minus = sfil_load_PNG_file("romfs:/res/Minus.png", SF2D_PLACE_RAM);
		button = sfil_load_PNG_file("romfs:/res/Button.png", SF2D_PLACE_RAM);
		setting = sfil_load_PNG_file("romfs:/res/Setting.png", SF2D_PLACE_RAM);
		bottomPopUp = sfil_load_PNG_file("romfs:/res/Bottom Pop-Up.png", SF2D_PLACE_RAM);
		pokemonBufferBox = sfil_load_PNG_file("romfs:/res/Pokemon Box.png", SF2D_PLACE_RAM);
		item = sfil_load_PNG_file("romfs:/res/item.png", SF2D_PLACE_RAM);
	} else {
		DSBottomBG = sfil_load_PNG_file("romfs:/res/Bottom BG.png", SF2D_PLACE_RAM);
		DSTopBG = sfil_load_PNG_file("romfs:/res/Top BG.png", SF2D_PLACE_RAM);
		DSBarSelected = sfil_load_PNG_file("romfs:/res/Bar Selected.png", SF2D_PLACE_RAM);
		DSBar = sfil_load_PNG_file("romfs:/res/Bar.png", SF2D_PLACE_RAM);
		DSEventBottom = sfil_load_PNG_file("romfs:/res/DS Menu Bottom BG.png", SF2D_PLACE_RAM);
		DSLangSelected = sfil_load_PNG_file("romfs:/res/Language Button Selected.png", SF2D_PLACE_RAM);
		DSLang = sfil_load_PNG_file("romfs:/res/Language Button.png", SF2D_PLACE_RAM);
		DSEventTop = sfil_load_PNG_file("romfs:/res/Event Database BG.png", SF2D_PLACE_RAM);
		DSNormalBarL = sfil_load_PNG_file("romfs:/res/Normal L.png", SF2D_PLACE_RAM);
		DSNormalBarR = sfil_load_PNG_file("romfs:/res/Normal R.png", SF2D_PLACE_RAM);
		DSSelectedBarL = sfil_load_PNG_file("romfs:/res/Selected L.png", SF2D_PLACE_RAM);
		DSSelectedBarR = sfil_load_PNG_file("romfs:/res/Selected R.png", SF2D_PLACE_RAM);
	}
}

void GUIGameElementsInit() {
	gameSelectorBottom2 = sfil_load_PNG_file("romfs:/res/LogosGen5.png", SF2D_PLACE_RAM);
	gameSelectorBottom1 = sfil_load_PNG_file("romfs:/res/LogosGen4.png", SF2D_PLACE_RAM);
	gameSelectorTop = sfil_load_PNG_file("romfs:/res/Logos.png", SF2D_PLACE_RAM);
}

void GUIGameElementsExit() {
	sf2d_free_texture(gameSelectorBottom2);
	sf2d_free_texture(gameSelectorBottom1);
	sf2d_free_texture(gameSelectorTop);
}

void GUIElementsExit() {
	sf2d_free_texture(noMove);
	sf2d_free_texture(hexIcon);
	sf2d_free_texture(hexBG);
	sf2d_free_texture(blueTextBox);
	sf2d_free_texture(otaButton);
	sf2d_free_texture(generationBG);
	sf2d_free_texture(includeInfoButton);
	sf2d_free_texture(hiddenPowerBG);
	sf2d_free_texture(ballsBG);
	sf2d_free_texture(male);
	sf2d_free_texture(female);
	sf2d_free_texture(naturestx);
	sf2d_free_texture(movesBottom);
	sf2d_free_texture(topMovesBG);
	sf2d_free_texture(editorBar);
	sf2d_free_texture(editorStatsBG);
	sf2d_free_texture(subArrow);
	sf2d_free_texture(backgroundTop);
	sf2d_free_texture(miniBox);
	sf2d_free_texture(minusButton);
	sf2d_free_texture(plusButton);
	sf2d_free_texture(balls);
	sf2d_free_texture(typesSheet);
	sf2d_free_texture(transferButton);
	sf2d_free_texture(bankTop);
	sf2d_free_texture(shinyStar);
	sf2d_free_texture(normalBar);
	sf2d_free_texture(RButton);
	sf2d_free_texture(LButton);
	sf2d_free_texture(creditsTop);
	sf2d_free_texture(pokeball);
	sf2d_free_texture(alternativeSpritesSmall);
	sf2d_free_texture(item);
	sf2d_free_texture(settings);
	sf2d_free_texture(DSEventBottom);
	sf2d_free_texture(DSLangSelected);
	sf2d_free_texture(DSLang);
	sf2d_free_texture(DSEventTop);
	sf2d_free_texture(DSNormalBarL);
	sf2d_free_texture(DSNormalBarR);
	sf2d_free_texture(DSSelectedBarL);
	sf2d_free_texture(DSSelectedBarR);
	sf2d_free_texture(DSBottomBG);
	sf2d_free_texture(DSTopBG);
	sf2d_free_texture(DSBarSelected);
	sf2d_free_texture(DSBar);
	sf2d_free_texture(bottomPopUp);
	sf2d_free_texture(pokemonBufferBox);
	sf2d_free_texture(selectorCloning);
	sf2d_free_texture(back);
	sf2d_free_texture(setting);
	sf2d_free_texture(editorBG);
	sf2d_free_texture(plus);
	sf2d_free_texture(minus);
	sf2d_free_texture(button);
	sf2d_free_texture(boxView);
	sf2d_free_texture(infoView);
	sf2d_free_texture(selector);
	sf2d_free_texture(warningTop);
	sf2d_free_texture(warningBottom);
	sf2d_free_texture(eventMenuBottomBar);
	sf2d_free_texture(eventMenuTopBarSelected);
	sf2d_free_texture(eventMenuTopBar);
	sf2d_free_texture(spritesSmall);
	sf2d_free_texture(darkButton);
	sf2d_free_texture(eventTop);
	sf2d_free_texture(left);
	sf2d_free_texture(lightButton);
	sf2d_free_texture(redButton);
	sf2d_free_texture(right);
	sf2d_free_texture(menuBar);
	sf2d_free_texture(menuSelectedBar);
	sftd_free_font(fontBold9);
	sftd_free_font(fontBold11);
	sftd_free_font(fontBold12);
	sftd_free_font(fontBold14);
	sftd_free_font(fontBold15);
	sftd_free_font(fontBold18);
	sftd_free_font(fontFixed);
	sftd_free_font(unicodeJPN12);
	sftd_free_font(unicodeKOR12);
}

void init_font_cache() {
	freezeMsgDetails("Caching font 1/9...");
	sftd_draw_text(unicodeJPN12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 2/9...");
	sftd_draw_text(unicodeKOR12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 3/9...");
	sftd_draw_text(fontBold18, 0, 0, RGBA8(0, 0, 0, 0), 18, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 4/9...");
	sftd_draw_text(fontBold14, 0, 0, RGBA8(0, 0, 0, 0), 14, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 5/9...");
	sftd_draw_text(fontBold15, 0, 0, RGBA8(0, 0, 0, 0), 15, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 6/9...");
	sftd_draw_text(fontBold12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 7/9...");
	sftd_draw_text(fontBold11, 0, 0, RGBA8(0, 0, 0, 0), 11, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 8/9...");
	sftd_draw_text(fontBold9, 0, 0, RGBA8(0, 0, 0, 0), 9, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	freezeMsgDetails("Caching font 9/9...");
	sftd_draw_text(fontFixed, 0, 0, RGBA8(0, 0, 0, 0), 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
}

void drawFPSDebug() {
	sf2d_draw_rectangle(10, 10, 108, 20, RGBA8(0, 0, 0, 200));
	sftd_draw_textf(fontBold12, 12, 13, WHITE, 12, "FPS: %2.6f", sf2d_get_fps());
}

void pksm_end_frame() {
	#if DEBUG
		drawFPSDebug();
	#endif
	sf2d_end_frame();
}

void printSelector(int x, int y, int width, int height) {
	sf2d_draw_rectangle(x, y, width, height, BUTTONBORD);
	sf2d_draw_rectangle(x + 1, y + 1, width - 2, height - 2, BUTTONGREY);
}

void printMenuTop() {
	sf2d_draw_rectangle(0, 0, 400, 240, MENUBLUE);
	sf2d_draw_rectangle(0, 0, 400, 25, HIGHBLUE);
}

void printMenuBottom() {
	sf2d_draw_rectangle(0, 0, 320, 240, PALEBLUE);
	sf2d_draw_rectangle(0, 219, 320, 21, MENUBLUE);
}

void infoDisp(char* message) {
	while (aptMainLoop()) {
		hidScanInput();

		if (hidKeysDown() & KEY_A) break;
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_texture(warningTop, 0, 0);
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, message);
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press A to continue.")) / 2, 130, WHITE, 12, "Press A to continue.");
		pksm_end_frame();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_texture(warningBottom, 0, 0);
		pksm_end_frame();
		sf2d_swapbuffers();
	}
}

int confirmDisp(char* message) {
	while (aptMainLoop()) {
		hidScanInput();

		if (hidKeysDown() & KEY_A) return 1;
		if (hidKeysDown() & KEY_B) return 0;
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_texture(warningTop, 0, 0);
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, message);
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press A to continue, B to cancel.")) / 2, 130, WHITE, 12, "Press A to continue, B to cancel.");
		pksm_end_frame();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_texture(warningBottom, 0, 0);
		pksm_end_frame();
		sf2d_swapbuffers();
	}
	return 0;
}
char* lastMessage;

void _freezeMsgWithDetails(char* message, char* details, bool useLastMessage) {
	if (!useLastMessage) {
		free(lastMessage);
		lastMessage = malloc(strlen(message)*sizeof(char)+1);
		strcpy(lastMessage, message);
		lastMessage[strlen(message)] = '\0';
	}

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(warningTop, 0, 0);
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, lastMessage)) / 2, 95, WHITE, 15, lastMessage);
		sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, details)) / 2, 130, WHITE, 12, details);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(warningBottom, 0, 0);
	pksm_end_frame();
	sf2d_swapbuffers();
}

void freezeMsgDetails(char* details) { _freezeMsgWithDetails("", details, true); }
void freezeMsg(char* message) { _freezeMsgWithDetails(message, "Please wait.", false); }
void freezeMsgWithDetails(char* message, char* details) { _freezeMsgWithDetails(message, details, false); }

void progressBar(char* message, u32 current, u32 sz) {
	char* progress = (char*)malloc(40 * sizeof(char));
	snprintf(progress, 40, "Progress: %lu/%lu bytes", current, sz);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(warningTop, 0, 0);
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, WHITE, 15, message);
		sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, progress)) / 2, 130, WHITE, 12, progress);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(warningBottom, 0, 0);
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(progress);
}

void drawMenuTop(int game) {
	if (game < 6) {
		printMenuTop();
		printTitle("PKSM");
		sf2d_draw_texture(pokeball, (400 - pokeball->width) / 2 + 5, (240 - pokeball->height) / 2 + 10);
	}
	else 
		sf2d_draw_texture(DSTopBG, 0, 0);
	
	sftd_draw_text(fontBold9, (398 - sftd_get_text_width(fontBold9, 9, VERSION)), 229, LIGHTBLUE, 9, VERSION);
}

void printAnimatedBG(bool isUp) {
	sf2d_draw_texture(backgroundTop, 0, 0);
	animateBG(isUp);
}

void printTitle(const char* title) {
	sftd_draw_text(fontBold14, (400 - sftd_get_text_width(fontBold14, 14, title)) / 2, 4, BLUE, 14, title);
}

void printBottomIndications(const char* message) {
	sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, message)) / 2, 225, LIGHTBLUE, 9, message);
}

void gameSelectorMenu(int n) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printMenuTop();
		sftd_draw_text(fontBold9, (400 - sftd_get_text_width(fontBold9, 9, "Cart has priority over digital copy.")) / 2, 6, BLUE, 9, "Cart has priority over digital copy.");
		
		for (int i = 0; i < 6; i++) {
			if (n == i) {
				sf2d_draw_texture_part_blend(gameSelectorTop, 26 + i*60 + 4, 80 + 4, logo_lookup6[i], 0, logo_lookup6[i+1] - logo_lookup6[i] - 1, 70, HIGHBLUE);
				sf2d_draw_texture_part_blend(gameSelectorTop, 26 + i*60, 80, logo_lookup6[i], 0, logo_lookup6[i+1] - logo_lookup6[i] - 1, 70, logoColors[i]);
			} else
				sf2d_draw_texture_part(gameSelectorTop, 26 + i*60, 80, logo_lookup6[i], 0, logo_lookup6[i+1] - logo_lookup6[i] - 1, 70);
		}

		sftd_draw_text(fontBold18, (400 - sftd_get_text_width(fontBold18, 18, gamesList[n])) / 2, 185, logoColors[n], 18, gamesList[n]);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		
		for (int i = 6; i < 11; i++) {
			if (n == i) {
				sf2d_draw_texture_part_blend(gameSelectorBottom1, 26 + (i - 6)*56 + 4, 40 + 4, logo_lookup4[i - 6], 0, logo_lookup4[i - 5] - logo_lookup4[i - 6] - 1, 55, HIGHBLUE);
				sf2d_draw_texture_part_blend(gameSelectorBottom1, 26 + (i - 6)*56, 40, logo_lookup4[i - 6], 0, logo_lookup4[i - 5] - logo_lookup4[i - 6] - 1, 55, logoColors[i]);
			} else
				sf2d_draw_texture_part(gameSelectorBottom1, 26 + (i - 6)*56, 40, logo_lookup4[i - 6], 0, logo_lookup4[i - 5] - logo_lookup4[i - 6] - 1, 55);
		}
		
		for (int i = 11; i < 15; i++) {
			if (n == i) {
				sf2d_draw_texture_part_blend(gameSelectorBottom2, 40 + (i - 11)*60 + 4, 115 + 4, logo_lookup5[i - 11], 0, logo_lookup5[i - 10] - logo_lookup5[i - 11] - 1, 63, HIGHBLUE);
				sf2d_draw_texture_part_blend(gameSelectorBottom2, 40 + (i - 11)*60, 115, logo_lookup5[i - 11], 0, logo_lookup5[i - 10] - logo_lookup5[i - 11] - 1, 63, logoColors[i]);
			} else
				sf2d_draw_texture_part(gameSelectorBottom2, 40 + (i - 11)*60, 115, logo_lookup5[i - 11], 0, logo_lookup5[i - 10] - logo_lookup5[i - 11] - 1, 63);
		}

		printBottomIndications("Move your DPAD. Press A to continue, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void menu3(int currentEntry, char* menu[], int n, bool isMain) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		if (isMain)
			sf2d_draw_texture(settings, 292, 194);
		for (int i = 0; i < 3; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(menuSelectedBar, (320 - menuSelectedBar->width) / 2, 60 + i * (menuSelectedBar->height));
			else
				sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 60 + i * (menuBar->height));
			sftd_draw_text(fontBold18, (320 - sftd_get_text_width(fontBold18, 18, menu[i])) / 2, 56 + (menuBar->height - 18) / 2 + i * (menuBar->height), (i == currentEntry) ? DARKBLUE : YELLOW, 18, menu[i]);
		}
		printBottomIndications(isMain ? "Press START to quit." : "Press A to select an option.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void mainMenuDS(int currentEntry) {
	char* menu[2] = {"EVENTS", "OTHER"};
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(7);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSBottomBG, 0, 0);
		for (int i = 0; i < 2; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(DSBarSelected, (320 - DSBarSelected->width) / 2, 66 + i * (DSBarSelected->height + 16) + i + 1);
			else
				sf2d_draw_texture(DSBar, (320 - DSBar->width) / 2, 66 + i * (DSBar->height + 16));
			sftd_draw_text(fontBold18, (320 - sftd_get_text_width(fontBold18, 18, menu[i])) / 2, 67 + (DSBar->height - 18) / 2 + i * (DSBar->height + 16), WHITE, 18, menu[i]);
		}
		printBottomIndications("Press START to quit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void menu4(int currentEntry, char* menu[], int n) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		
		for (int i = 0; i < n; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(menuSelectedBar, (320 - menuSelectedBar->width) / 2, 40 + i * (menuSelectedBar->height));
			else
				sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 40 + i * (menuBar->height));
			sftd_draw_text(fontBold18, (320 - sftd_get_text_width(fontBold18, 18, menu[i])) / 2, 44 + i * (menuBar->height), (i == currentEntry) ? DARKBLUE : YELLOW, 18, menu[i]);
		}
		printBottomIndications("Press A to select an option.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printCredits() {
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	memset(buf, 0, 1500);
	loadFile(buf, "romfs:/res/credits.txt");
	
	while (aptMainLoop() && !(hidKeysUp() & KEY_B)) {
		hidScanInput();
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			printMenuTop();
			printTitle("Credits");
			sf2d_draw_texture_part(creditsTop, 16, 101, 0, 22, 181, 46);
			sf2d_draw_texture(creditsTop, 0, 45);
			sftd_draw_text(fontBold15, 18, 77, LIGHTBLUE, 15, "Bernardo Giordano");
			sftd_draw_text(fontBold15, 34, 113, LIGHTBLUE, 15, "Anty-Lemon");
			sftd_draw_text(fontBold15, 64, 174, LIGHTBLUE, 15, "dsoldier for the complete GUI design");
		pksm_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			printMenuBottom();
			sftd_draw_text(fontBold9, 20, 30, LIGHTBLUE, 9, (char*)buf);
			printBottomIndications("Press B to return.");
		pksm_end_frame();
		sf2d_swapbuffers();
	}
	
	free(buf);
}

void printDatabase6(char *database[], int currentEntry, int page, int spriteArray[]) {
	int pk, y = 41;
	char *pages = (char*)malloc(10 * sizeof(char));
	snprintf(pages, 10, "%d/%d", page + 1, 205);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printMenuTop();
		printTitle("Event Database");
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 20, y - ((i == currentEntry) ? movementOffsetSlow(2) : 0), 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 54, y + 12, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 54, y + 3, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, 148, database[page * 10 + i]);
			
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 202, y - ((i == currentEntry) ? movementOffsetSlow(2) : 0), 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, 148, database[page * 10 + i]);
			
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		sf2d_draw_texture(eventMenuBottomBar, (320 - eventMenuBottomBar->width) / 2, 45);
		sf2d_draw_texture(LButton, 83, 52);
		sf2d_draw_texture(RButton, 221, 52);
		sftd_draw_text(fontBold12, (320 - sftd_get_text_width(fontBold12, 12, pages)) / 2, 52, WHITE, 12, pages);
		printBottomIndications("Press A to continue, B to return.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pages);
}

void printDatabase5(char *database[], int currentEntry, int page, int spriteArray[], bool isSelected, int langSelected, bool langVett[]) {
	int pk, y = 41;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(DSEventTop, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarL, 18, y);
			else
				sf2d_draw_texture(DSNormalBarL, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 22, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 55, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 55, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarL->height;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarR, 200, y);
			else
				sf2d_draw_texture(DSNormalBarR, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 204, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarR->height;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSEventBottom, 0, 0);
		sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.")) / 2, 222, RGBA8(255, 255, 255, 130), 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.");
		
		if (isSelected) {
			char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
			char *path = (char*)malloc(40 * sizeof(char));
			u8* buf = (u8*)malloc(1500 * sizeof(u8));
			memset(buf, 0, 1500);
			snprintf(path, 40, "romfs:/database/gen5/%d.txt", page * 10 + currentEntry);
			loadFile(buf, path);
			
			sftd_draw_text(fontFixed, 8, 2,  WHITE, 10, (char*)buf);
			
			for (int t = 0; t < 7; t++) {
				int x = 0, y = 178;
				switch (t) {
					case 0 : { x = 25; break; }
					case 1 : { x = 63; break; }
					case 2 : { x = 101; break; }
					case 3 : { x = 139; break; }
					case 4 : { x = 177; break; }
					case 5 : { x = 215; break; }
					case 6 : { x = 253; break; }
				}
				
				sf2d_draw_texture(DSLang, x, y);
				if (langVett[t]) {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, WHITE, 12, languages[t]);
				}
				else {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, RGBA8(255, 255, 255, 100), 12, languages[t]);
				}
			}

			free(path);
			free(buf);
		}
	
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printDatabase4(char *database[], int currentEntry, int page, int spriteArray[], bool isSelected, int langSelected, bool langVett[]) {
	int pk, y = 41;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(DSEventTop, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarL, 18, y);
			else
				sf2d_draw_texture(DSNormalBarL, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 22, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 55, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 55, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarL->height;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarR, 200, y);
			else
				sf2d_draw_texture(DSNormalBarR, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 204, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarR->height;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSEventBottom, 0, 0);
		sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.")) / 2, 222, RGBA8(255, 255, 255, 130), 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.");
		
		if (isSelected) {
			char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
			char *path = (char*)malloc(40 * sizeof(char));
			u8* buf = (u8*)malloc(1500 * sizeof(u8));
			memset(buf, 0, 1500);
			snprintf(path, 40, "romfs:/database/gen4/%d.txt", page * 10 + currentEntry);
			loadFile(buf, path);
			
			sftd_draw_text(fontFixed, 8, 2,  WHITE, 10, (char*)buf);
			
			for (int t = 0; t < 7; t++) {
				int x = 0, y = 178;
				switch (t) {
					case 0 : { x = 25; break; }
					case 1 : { x = 63; break; }
					case 2 : { x = 101; break; }
					case 3 : { x = 139; break; }
					case 4 : { x = 177; break; }
					case 5 : { x = 215; break; }
					case 6 : { x = 253; break; }
				}
				
				sf2d_draw_texture(DSLang, x, y);
				if (langVett[t]) {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, WHITE, 12, languages[t]);
				}
				else {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, RGBA8(255, 255, 255, 100), 12, languages[t]);
				}
			}

			free(path);
			free(buf);
		}
	
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printDB7(int sprite, int i, bool langVett[], bool adapt, bool overwrite, int langSelected, int nInjected) {
	char *languages[] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR", "CHS", "CHT"};
	char *path = (char*)malloc(30 * sizeof(char));
	char *cont = (char*)malloc(3 * sizeof(char));
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	memset(buf, 0, 1499);
	snprintf(path, 30, "romfs:/database/%d.txt", i);
	snprintf(cont, 3, "%d", nInjected + 1);
	loadFile(buf, path);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printAnimatedBG(true);
		sf2d_draw_texture(eventTop, 0, 24);
		if (sprite != -1)
			sf2d_draw_texture_part_scale(spritesSmall, 282, 46 - movementOffsetLong(6), 40 * (sprite % 25) + 4, 30 * (sprite / 25), 34, 30, 2, 2);
		sftd_draw_text(fontFixed, 5, 28,  WHITE, 10, (char*)buf);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		sftd_draw_text(fontBold14, 16, 50, LIGHTBLUE, 14, "Languages:");
		sftd_draw_text(fontBold14, 33, 112, LIGHTBLUE, 14, "Overwrite Wondercard:");
		sftd_draw_text(fontBold14, 33, 140, LIGHTBLUE, 14, "Adapt language to WC:");
		sftd_draw_text(fontBold14, 33, 170, LIGHTBLUE, 14, "Inject WC in slot:");
		
		for (int t = 0; t < 9; t++) {
			int x = 0, y = 0;
			switch (t) {
				case 0 : { x = 114; y = 50; break; }
				case 1 : { x = 153; y = 50; break; }
				case 2 : { x = 192; y = 50; break; }
				case 3 : { x = 231; y = 50; break; }
				case 4 : { x = 270; y = 50; break; }
				case 5 : { x = 133; y = 74; break; }
				case 6 : { x = 172; y = 74; break; }
				case 7 : { x = 211; y = 74; break; }
				case 8 : { x = 250; y = 74; break; }
			}
			if (langVett[t]) {
				if (t == langSelected) {
					sf2d_draw_texture(redButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
				} else {
					sf2d_draw_texture(darkButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, YELLOW, 14, languages[t]);
				}
			}
			else {
				sf2d_draw_texture(lightButton, x, y);
				sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
			}
		}
			
		if (overwrite) {
			sf2d_draw_texture(redButton, 210, 110);
			sf2d_draw_texture(darkButton, 249, 110);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 110);
			sf2d_draw_texture(redButton, 249, 110);			
		}
		
		if (adapt) {
			sf2d_draw_texture(redButton, 210, 138);
			sf2d_draw_texture(darkButton, 249, 138);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 138);
			sf2d_draw_texture(redButton, 249, 138);
		}
		
		sf2d_draw_texture(darkButton, 229, 168);	
		
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 112, (overwrite) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 112, (!overwrite) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 140, (adapt) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 140, (!adapt) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 229 + (36 - sftd_get_text_width(fontBold14, 14, cont)) / 2, 170, YELLOW, 14, cont);
		
		printBottomIndications("Press START to inject, B to return.");
		pksm_end_frame();
	sf2d_swapbuffers();
	
	free(buf);
	free(cont);
	free(path);
}

void printDB6(int sprite, int i, bool langVett[], bool adapt, bool overwrite, int langSelected, int nInjected) {
	char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
	char *path = (char*)malloc(30 * sizeof(char));
	char *cont = (char*)malloc(3 * sizeof(char));
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	memset(buf, 0, 1499);
	snprintf(path, 30, "romfs:/database/%d.txt", i);
	snprintf(cont, 3, "%d", nInjected + 1);
	loadFile(buf, path);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printAnimatedBG(true);
		sf2d_draw_texture(eventTop, 0, 24);
		if (sprite != -1)
			sf2d_draw_texture_part_scale(spritesSmall, 282, 46 - movementOffsetLong(6), 40 * (sprite % 25) + 4, 30 * (sprite / 25), 34, 30, 2, 2);
		sftd_draw_text(fontFixed, 5, 28,  WHITE, 10, (char*)buf);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		sftd_draw_text(fontBold14, 33, 50, LIGHTBLUE, 14, "Languages:");
		sftd_draw_text(fontBold14, 33, 112, LIGHTBLUE, 14, "Overwrite Wondercard:");
		sftd_draw_text(fontBold14, 33, 140, LIGHTBLUE, 14, "Adapt language to WC:");
		sftd_draw_text(fontBold14, 33, 170, LIGHTBLUE, 14, "Inject WC in slot:");
		
		for (int t = 0; t < 7; t++) {
			int x = 0, y = 0;
			switch (t) {
				case 0 : { x = 132; y = 50; break; }
				case 1 : { x = 171; y = 50; break; }
				case 2 : { x = 210; y = 50; break; }
				case 3 : { x = 249; y = 50; break; }
				case 4 : { x = 151; y = 74; break; }
				case 5 : { x = 190; y = 74; break; }
				case 6 : { x = 229; y = 74; break; }
			}
			if (langVett[t]) {
				if (t == langSelected) {
					sf2d_draw_texture(redButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
				} else {
					sf2d_draw_texture(darkButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, YELLOW, 14, languages[t]);
				}
			}
			else {
				sf2d_draw_texture(lightButton, x, y);
				sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
			}
		}
			
		if (overwrite) {
			sf2d_draw_texture(redButton, 210, 110);
			sf2d_draw_texture(darkButton, 249, 110);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 110);
			sf2d_draw_texture(redButton, 249, 110);			
		}
		
		if (adapt) {
			sf2d_draw_texture(redButton, 210, 138);
			sf2d_draw_texture(darkButton, 249, 138);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 138);
			sf2d_draw_texture(redButton, 249, 138);
		}
		
		sf2d_draw_texture(darkButton, 229, 168);	
		
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 112, (overwrite) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 112, (!overwrite) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 140, (adapt) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 140, (!adapt) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 229 + (36 - sftd_get_text_width(fontBold14, 14, cont)) / 2, 170, YELLOW, 14, cont);
		
		printBottomIndications("Press START to inject, B to return.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(buf);
	free(cont);
	free(path);
}

void printEditor(u8* mainbuf, int game, int currentEntry, int langCont) {
	int y = 41;
	char *languages[] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR", "CHS", "CHT"};

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printMenuTop();
		printTitle("Save File Editor");
		
		for (int i = 0; i < 5; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			switch (i) {
				case 0 : {
					int x = 140;
					sftd_draw_text(fontBold12, 40, y + 10, BLACK, 12, "Language:");
					sf2d_draw_texture(miniBox, x, y + 7);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[langCont])) / 2, y + 10, YELLOW, 12, languages[langCont]);
					break;
				}
			}
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			switch (i) {
				case 5 : { sftd_draw_text(fontBold12, 200 + (182 - sftd_get_text_width(fontBold12, 12, "Clear Mystery Gift box")) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, "Clear Mystery Gift box"); break; }
			}
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		printBottomIndications("Press START to edit, A to toggle, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

u16 getAlternativeSprite(u8* pkmn, int game) {
	u8 form = getForm(pkmn);
	if (form) {
		FormData *forms = getLegalFormData(getPokedexNumber(pkmn), game);
		int spritenum = forms->spriteNum;
		if (spritenum > 0 && form >= forms->min && form <= forms->max)
			spritenum += form - (forms->min > 0 ? forms->min : 1);
		free(forms);
		return spritenum;
	}
	return 0;
}

void printElement(u8* pkmn, int game, u16 n, int x, int y) {
	u16 t = getAlternativeSprite(pkmn, game);
	if (t) {
		t -= 1;
		sf2d_draw_texture_part(alternativeSpritesSmall, x, y, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30); 
	} else {
		if (getPokedexNumber(pkmn) < 0 || getPokedexNumber(pkmn) > 821)
			sf2d_draw_texture_part(spritesSmall, x, y, 0, 0, 34, 30);
		else
			sf2d_draw_texture_part(spritesSmall, x, y, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30);
	}
	if (isEgg(pkmn))
		sf2d_draw_texture_part(spritesSmall, x + 6, y + 6, 40 * (EGGSPRITEPOS % 25) + 4, 30 * (EGGSPRITEPOS / 25), 34, 30);
	if (getItem(pkmn))
		sf2d_draw_texture(item, x + 3, y + 21);
}
void printElementBlend(u8* pkmn, int game, u16 n, int x, int y, u32 color) {
	u16 t = getAlternativeSprite(pkmn, game);
	if (t) {
		t -= 1;
		sf2d_draw_texture_part_blend(alternativeSpritesSmall, x, y, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, color); 
	} else {
		if (getPokedexNumber(pkmn) < 0 || getPokedexNumber(pkmn) > 821)
			sf2d_draw_texture_part_blend(spritesSmall, x, y, 0, 0, 34, 30, color);
		else
			sf2d_draw_texture_part_blend(spritesSmall, x, y, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, color);
	}
	if (isEgg(pkmn))
		sf2d_draw_texture_part_blend(spritesSmall, x + 6, y + 6, 40 * (EGGSPRITEPOS % 25) + 4, 30 * (EGGSPRITEPOS / 25), 34, 30, color);
	if (getItem(pkmn))
		sf2d_draw_texture_blend(item, x + 3, y + 21, color);
}

void infoViewer(u8* pkmn, int game) {
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	int y_desc = 29;
	char* entries[] = {"Nickname:", "OT:", "Pokerus:", "Nature", "Ability:", "Item:", "ESV / TSV:", "TID / SID:", "HT/OT Fr.ship:", "Hidden Power:"};
	char* values[] = {"HP", "Attack", "Defense", "Sp. Atk", "Sp. Def", "Speed"};
	
	printAnimatedBG(true);
	sf2d_draw_texture(infoView, 0, 2);

	sf2d_draw_texture((getMove(pkmn, 0)) ? normalBar : noMove, 252, 155);
	sf2d_draw_texture((getMove(pkmn, 1)) ? normalBar : noMove, 252, 176);
	sf2d_draw_texture((getMove(pkmn, 2)) ? normalBar : noMove, 252, 197);
	sf2d_draw_texture((getMove(pkmn, 3)) ? normalBar : noMove, 252, 218);
	
	sftd_draw_text(fontBold12, 251, 138, WHITE, 12, "Moves");
	for (int i = 0; i < 10; i++) {
		if (i == 8 && isEgg(pkmn))
			sftd_draw_text(fontBold12, 2, y_desc, BLUE, 12, "Egg cycle:");
		else
			sftd_draw_text(fontBold12, 2, y_desc, BLUE, 12, entries[i]);
		y_desc += 20;
		if (i == 2) y_desc += 5;
		if (i == 5) y_desc += 6;
	}
	
	y_desc = 8;
	for (int i = 0; i < 6; i++) {
		sftd_draw_text(fontBold12, 225, y_desc, LIGHTBLUE, 12, values[i]);
		y_desc += 20;
	}
	
	if (getPokedexNumber(pkmn) > 0 && getPokedexNumber(pkmn) < 822) {
		sf2d_draw_texture_part(balls, -2, -5, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
		sftd_draw_wtext(fontBold12, 30, 6, WHITE, 12, listSpecies.items[getPokedexNumber(pkmn)]);
		
		if (getGender(pkmn) == 0)
			sf2d_draw_texture(male, 146, 7);
		else if (getGender(pkmn) == 1)
			sf2d_draw_texture(female, 148, 7);
		
		char* level = (char*)malloc(8 * sizeof(char));
		snprintf(level, 8, "Lv.%u", getLevel(pkmn));
		sftd_draw_text(fontBold12, 160, 6, WHITE, 12, level);
		free(level);
		
		u32 nick[NICKNAMELENGTH*2];
		memset(nick, 0, NICKNAMELENGTH*2);
		getNickname(pkmn, nick);
		sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 215 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)nick)), 29, WHITE, 12, (wchar_t*)nick);
		
		u32 ot_name[NICKNAMELENGTH*2];
		memset(ot_name, 0, NICKNAMELENGTH*2);
		getOT(pkmn, ot_name);
		sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 215 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)ot_name)), 49, WHITE, 12, (wchar_t*)ot_name);
		
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, (isInfected(pkmn) ? "Yes" : "No")), 69, WHITE, 12, isInfected(pkmn) ? "Yes" : "No");
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, natures[getNature(pkmn)]), 94, WHITE, 12, natures[getNature(pkmn)]);
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, abilities[getAbility(pkmn)]), 114, WHITE, 12, abilities[getAbility(pkmn)]);
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, items[getItem(pkmn)]), 134, WHITE, 12, items[getItem(pkmn)]);
		
		if (isShiny(pkmn))
			sf2d_draw_texture(shinyStar, 205, 9);
		
		char* friendship = (char*)malloc(11 * sizeof(char));
		if (isEgg(pkmn))
			snprintf(friendship, 11, "%u", getOTFriendship(pkmn));
		else
			snprintf(friendship, 11, "%u / %u", getHTFriendship(pkmn), getOTFriendship(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, friendship), 200, WHITE, 12, friendship);
		free(friendship);
		
		char* otid = (char*)malloc(18 * sizeof(char));
		snprintf(otid, 18, "%u / %u", getPSV(pkmn), getTSV(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, otid), 160, WHITE, 12, otid);
		snprintf(otid, 18, "%u / %u", getOTID(pkmn), getSOTID(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, otid), 180, WHITE, 12, otid);
		free(otid);

		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, hpList[getHPType(pkmn)]), 220, WHITE, 12, hpList[getHPType(pkmn)]);
		
		int max = sftd_get_text_width(fontBold12, 12, "252");		
		int y_moves = 159;
		for (int i = 0; i < 4; i++) {
			if (getMove(pkmn, i))
				sftd_draw_wtext(fontBold12, 396 - sftd_get_wtext_width(fontBold12, 12, moves[getMove(pkmn, i)]), y_moves, WHITE, 12, moves[getMove(pkmn, i)]);
			y_moves += 21;
		}
		
		char* tmp = (char*)malloc(4);
		for (int i = 0; i < 6; i++) {
			snprintf(tmp, 4, "%d", getIV(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 289 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
			snprintf(tmp, 4, "%d", getEV(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 328 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
			snprintf(tmp, 4, "%d", getStat(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 369 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
		}
		free(tmp);
	}
}

void printPKViewer(u8* mainbuf, u8* tmp, bool isTeam, int game, int currentEntry, int menuEntry, int box, int mode, int additional1, int additional2) {
	char* menuEntries[] = {"EDIT", "CLONE", "RELEASE", "GENERATE", "EXIT"};
	int x;
	char* page = (char*)malloc(7 * sizeof(char));
	snprintf(page, 7, "Box %d", box + 1);
	
	u8* pkmn = (u8*)malloc(PKMNLENGTH * sizeof(u8));
	getPkmn(mainbuf, (isTeam) ? 33 : box, currentEntry, pkmn, game);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		if (mode == ED_GENERATE) {
			char* temp = (char*)malloc(4);
			sf2d_draw_texture(generationBG, 0, 0);
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 8; j++) {
					if (additional1 == i * 8 + j)
						printSelector(j*49 + j, i*47 + i, 49, 47);
					sf2d_draw_texture_part(spritesSmall, 7 + 49 * j + j, 2 + 47 * i + i, 40 * ((40 * additional2 + i * 8 + j + 1) % 25) + 4, 30 * ((40 * additional2 + i * 8 + j + 1) / 25), 34, 30);
					snprintf(temp, 4, "%d", 40 * additional2 + i * 8 + j + 1);
					sftd_draw_text(fontBold9, 49 * j + (49 - sftd_get_text_width(fontBold9, 9, temp)) / 2 + j, 34 + i * 47 + i, WHITE, 9, temp);
				}
			}
			free(temp);
		} else
			infoViewer(pkmn, game);
		
		if (mode == ED_OTA) {
			sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, 220));
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Launch the client on your PC...")) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, "Launch the client on your PC...");
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press B to exit.")) / 2, 130, WHITE, 12, "Press B to exit.");
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(boxView, 0, 0);
		sf2d_draw_texture(editorBar, 0, 210);
		sftd_draw_text(fontBold12, 12 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 19, WHITE, 12, page);
		sf2d_draw_texture(left, 7, 17);
		sf2d_draw_texture(right, 185, 17);
		sf2d_draw_texture(otaButton, 240, 211);
		sf2d_draw_texture(back, 283, 211);
		sftd_draw_text(fontBold12, 247, 7, WHITE, 12, "Team");
		
		int y = 45;
		int pointer[2] = {0, 0};
		for (int i = 0; i < 5; i++) {
			x = 4;
			for (int j = 0; j < 6; j++) {
				getPkmn(mainbuf, box, i * 6 + j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n > 0 && n < 822)
					printElement(pkmn, game, n, x, y);

				if ((currentEntry == (i * 6 + j)) && !isTeam) {
					pointer[0] = x + 18;
					pointer[1] = y - 8;
				}
				x += 34;
			}
			y += 30;
		}
		
		for (int i = 0; i < 3; i++) {
			sf2d_draw_texture(pokemonBufferBox, 214, 40 + i * 45);
			sf2d_draw_texture(pokemonBufferBox, 266, 60 + i * 45);
		}
		
		y = 45;
		for (int i = 0; i < 3; i++) {
			x = 222;
			for (int j = 0; j < 2; j++) {
				getPkmn(mainbuf, 33, i * 2 + j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n)
					printElement(pkmn, game, n, x, (j == 1) ? y + 20 : y);

				if ((currentEntry == (i * 2 + j)) && isTeam) {
					pointer[0] = x + 18;
					pointer[1] = y - 8 + ((j == 1) ? 20 : 0);
				}
				x += 52;
			}
			y += 45;
		}
		if (mode != ED_CLONE)
			sf2d_draw_texture(selector, pointer[0], pointer[1] - ((mode == ED_STANDARD) ? movementOffsetSlow(3) : 0));
		else {
			sf2d_draw_texture(selectorCloning, pointer[0], pointer[1] - movementOffsetSlow(3));			
			sf2d_draw_texture(bottomPopUp, 1, 214);
			sftd_draw_text(fontBold11, 8, 220, WHITE, 11, "Press A to clone in selected slot, B to cancel");
		}
		
		if (mode == ED_MENU) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sf2d_draw_texture(bottomPopUp, 1, 214);
			sf2d_draw_texture(includeInfoButton, 242, 5);
			sftd_draw_wtextf(fontBold11, 8, 220, WHITE, 11, L"%s has been selected.", listSpecies.items[getPokedexNumber(tmp)]);
			for (int i = 0; i < 5; i++) {
				sf2d_draw_texture(button, 208, 42 + i * 27 + i*4);
				if (isTeam && (i == 0 || i == 2 || i == 3))
					sf2d_draw_texture_blend(button, 208, 42 + i * 27 + i*4, RGBA8(0, 0, 0, 100));
				sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, menuEntries[i])) / 2, 49 + i * 27 + i*4, BLACK, 12, menuEntries[i]);
				if (i == menuEntry)
					sf2d_draw_texture(subArrow, 203 - movementOffsetSlow(3), 46 + i * 27 + i*4);
			}
		} else if (mode == ED_GENERATE) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Generate with A in the top screen.")) / 2, 105, WHITE, 14, "Generate with A in the top screen.");
		} else if (mode == ED_OTA) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			drawIP(fontBold9);
		} else if (mode != ED_CLONE) {
			if (mode == ED_STANDARD)
				sftd_draw_textf(fontBold9, 10, 220, BARTEXT, 9, "TID: %u / SID: %u / TSV: %u", getSaveTID(mainbuf, game), getSaveSID(mainbuf, game), getSaveTSV(mainbuf, game));	
			else
				sftd_draw_textf(fontBold9, 10, 220, BARTEXT, 9, "Seed: %lx %lx %lx %lx", getSaveSeed(mainbuf, game, 3), getSaveSeed(mainbuf, game, 2), getSaveSeed(mainbuf, game, 1), getSaveSeed(mainbuf, game, 0));	
		}
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pkmn);
	free(page);
}

void printPKEditor(u8* pkmn, int game, int additional1, int additional2, int additional3, int mode, char* descriptions[]) {
	int max = sftd_get_text_width(fontBold12, 12, "252");
	char* entries[] = {"Level:", "Nature:", "Ability:", "Item:", "Shiny:", "Pokerus:", "OT:", "Nickname:", "Friendship:"};
	char* options[] = {"STATS", "MOVES", "SAVE"};
	
	char* values[6] = {"HP:", "Attack:", "Defense:", "Sp. Attack:", "Sp. Defense:", "Speed:"};
	u16 n = getPokedexNumber(pkmn);
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	if (mode == ED_BASE || mode == ED_STATS) {
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Edit your Pokemon in the bottom screen")) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, "Edit your Pokemon in the bottom screen");
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Tap SAVE when you're done")) / 2, 115, RGBA8(255, 255, 255, giveTransparence()), 15, "Tap SAVE when you're done");
	} else if (mode == ED_ITEMS) {
		int entry;
		int y = 0;
		sf2d_draw_texture(topMovesBG, 0, 0);
		for (int i = 0; i < 20; i++) {
			entry = 40 * additional2 + i;
			if (i == additional1)
				printSelector(2, y, 198, 11);
			else if (i + 20 == additional1) 
				printSelector(200, y, 198, 11);
			sftd_draw_wtextf(fontBold9, 2, y, WHITE, 9, L"%d - %ls", entry, itemsSorted[entry]);
			sftd_draw_wtextf(fontBold9, 200, y, WHITE, 9, L"%d - %ls", entry + 20, itemsSorted[entry + 20]);
			y += 12;
		}	
	} else if (mode == ED_MOVES) {

		int entry;
		int y = 0;
		sf2d_draw_texture(topMovesBG, 0, 0);
		for (int i = 0; i < 20; i++) {
			entry = 40 * additional2 + i;
			if (i == additional1) 
				printSelector(2, y, 198, 11);
			else if (i + 20 == additional1) 
				printSelector(200, y, 198, 11);
			sftd_draw_wtextf(fontBold9, 2, y, WHITE, 9, L"%d - %ls", entry, movesSorted[entry]);
			sftd_draw_wtextf(fontBold9, 200, y, WHITE, 9, L"%d - %ls", entry + 20, movesSorted[entry + 20]);
			y += 12;
		}
	} else if (mode == ED_NATURES) {
		char* hor[] = {"Neutral", "-Attack", "-Defense", "-Speed", "-Sp. Atk.", "-Sp. Def."};
		char* ver[] = {"+Attack", "+Defense", "+Speed", "+Sp. Atk.", "+Sp. Def."};
		
		sf2d_draw_texture(naturestx, 0, 0);
		for (int i = 0; i < 6; i++)
			sftd_draw_text(fontBold12, 66 * i + (66 - sftd_get_text_width(fontBold12, 12, hor[i])) / 2, 13, (i == 0) ? YELLOW : BLUE, 12, hor[i]);
		for (int i = 0; i < 5; i++)
			sftd_draw_text(fontBold12, (66 - sftd_get_text_width(fontBold12, 12, ver[i])) / 2, 53 + i * 40, BLUE, 12, ver[i]);
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (additional1 == i * 5 + j)
					printSelector(66*j + 66 + j, 40*i + 40, 66, 39);
				sftd_draw_wtext(fontBold12, 66 + 66 * j + (66 - sftd_get_wtext_width(fontBold12, 12, natures[i * 5 + j])) / 2 + j, 53 + i * 40, (i == j) ? YELLOW : WHITE, 12, natures[i * 5 + j]);
			}
		}
	} else if (mode == ED_BALLS) {
		char* entries[] = {"Master Ball", "Ultra Ball", "Great Ball", "Poke Ball", "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball", "Repeat Ball", "Timer Ball", "Luxury Ball", "Premier Ball", "Dusk Ball", "Heal Ball", "Quick Ball", "Cherish Ball", "Fast Ball", "Level Ball", "Lure Ball", "Heavy Ball", "Love Ball", "Friend Ball", "Moon Ball", "Sport Ball", "Dream Ball", "Beast Ball", " ", " ", " ", " "};
		sf2d_draw_texture(ballsBG, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 6; j++) {
				if (additional1 == i * 6 + j)
					printSelector(66*j + j, 47*i + i, 66, 47);
				sf2d_draw_texture_part(balls, 17 + 66 * j + j, 2 + i * 47, 32 * ((i * 6 + j + 1) % 8), 32 * ((i * 6 + j + 1) / 8), 32, 32);
				sftd_draw_text(fontBold9, 66 * j + (66 - sftd_get_text_width(fontBold9, 9, entries[i * 6 + j])) / 2 + j, 30 + i * 47 + i, WHITE, 9, entries[i * 6 + j]);
			}
		}
	} else if (mode == ED_HIDDENPOWER) {
		sf2d_draw_texture(hiddenPowerBG, 0, 0);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (additional1 == i * 4 + j)
					printSelector(99*j + j, 59*i + i, 99, 59);
				sf2d_draw_texture_part(typesSheet, 24 + 99 * j + j, 20 + 59 * i + i, 50 * (i * 4 + j + 1), 0, 50, 18); 
			}
		}
	} else if (mode == ED_FORMS) {
		char* entries[] = {"Default", "Alolan", "Default", "Rock Star", "Belle", "Pop Star", "Ph.D", "Libre", "Cosplay", "Default", "Original", "Hoenn", "Sinnoh", "Unova", "Kalos", "Alola", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "?", "Normal", "Attack", "Defense", "Speed", "Plant", "Sandy", "Trash", "West Sea", "East Sea", "Default", "Heat", "Wash", "Fridge", "Fan", "Mow", "Altered", "Origin", "Land", "Sky", "Red-Striped", "Blue-Striped", "Spring", "Summer", "Autumn", "Winter", "Incarnate", "Therian", "Default", "White", "Black", "Ordinary", "Resolute", "Aria", "Pirouette", "Default", "Battle Bond", "Icy Snow", "Polar", "Tundra", "Continental", "Garden", "Elegant", "Meadow", "Modern", "Marine", "Archipelago", "High Plains", "Sandstorm", "River", "Monsoon", "Savanna", "Sun", "Ocean", "Jungle", "Fancy", "Poke Ball", "Red", "Yellow", "Orange", "Blue", "White", "Eternal Flower", "Natural", "Heart", "Star", "Diamond", "Debutante", "Matron", "Dandy", "La Reine", "Kabuki", "Pharaoh", "Average", "Small", "Large", "Super", "50%", "10%", "10%-PC", "50%-PC", "Confined", "Unbound", "Baile", "Pom-Pom", "Pa'u", "Sensu", "Midday", "Midnight", "Red", "Orange", "Yellow", "Green", "Blue", "Indigo", "Violet", "Default", "Original Color", "Default", "Spiky-Eared", "Default", "Female"};
		FormData *forms = getLegalFormData((u16)additional2, game);
		int numforms = forms->max - forms->min + 1;
		
		int rows, columns, width, height;
		if (numforms <= 16) {
			columns = 4; rows = 4;
			width = 99; height = 59;
			sf2d_draw_texture(hiddenPowerBG, 0, 0);
		} else {
			columns = 6; rows = 5;
			width = 66; height = 47;
			sf2d_draw_texture(ballsBG, 0, 0);
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (additional1 == i * columns + j)
					printSelector(j*width + j, i*height + i, width, height);

				int form = i * columns + j;
				if (form < numforms) {
					char *str = entries[forms->stringNum + form];
					if (forms->min > 0)
						form++;
					if (form == 0 || forms->spriteNum == 0)
						sf2d_draw_texture_part(spritesSmall, (width - 34) / 2 + width * j + j, 2 * (height - 44) / 3 + i * height, 40 * (additional2 % 25) + 4, 30 * (additional2 / 25), 34, 30);
					else {
						int sprite = forms->spriteNum + form - 2;
						sf2d_draw_texture_part(alternativeSpritesSmall, (width - 34) / 2 + width * j + j, 2 * (height - 44) / 3 + i * height + i, 40 * (sprite % 6) + 4, 30 * (sprite / 6), 34, 30);
					}
					sftd_draw_text(fontBold9, width * j + (width - sftd_get_text_width(fontBold9, 9, str)) / 2 + j, (height * 5) / 7 + i * height + i, WHITE, 9, str);
				}
			}
		}
	} else if (mode == ED_HEX) {
		sf2d_draw_texture(hexBG, 0, 0);
		if (hax)
			sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(255, 0, 0, 100));
		for (int rows = 0; rows < 15; rows++) {
			for (int columns = 0; columns < 16; columns++) {
				int byte = rows*16 + columns;
				if (additional1 == byte)
					printSelector(columns*25, rows*15, 24, 14);
				sftd_draw_textf(fontBold11, 4 + 25*columns, 15*rows, (sector[byte][0]) ? WHITE : DS, 11, "%02hhX", pkmn[byte]);
				
				if (byte == 231) break;
			}
		}
		sftd_draw_textf(fontBold11, 4, 225, LIGHTBLUE, 11, "%s", descriptions[additional1]);
	}
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		if (mode != ED_HEX)
			printAnimatedBG(false);
		if (mode == ED_BASE || mode == ED_ITEMS || mode == ED_NATURES || mode == ED_BALLS || mode == ED_FORMS) {
			sf2d_draw_texture(editorBG, 0, 1);
			sf2d_draw_texture(editorBar, 0, 210);
			sf2d_draw_texture(hexIcon, 290, 1);
			
			sftd_draw_wtext(fontBold12, 27, 4, WHITE, 12, listSpecies.items[n]);
			sf2d_draw_texture_part(balls, -2, -6, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
			
			u16 t = getAlternativeSprite(pkmn, game);
			int ofs = movementOffsetSlow(3);
			if (t) {
				t -= 1;
				sf2d_draw_texture_part_scale_blend(alternativeSpritesSmall, 232, 32 - ofs, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, 2, 2, RGBA8(0x0, 0x0, 0x0, 100)); 
				sf2d_draw_texture_part_scale(alternativeSpritesSmall, 227, 27 - ofs, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, 2, 2);
			} else {
				sf2d_draw_texture_part_scale_blend(spritesSmall, 232, 32 - ofs, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, 2, 2, RGBA8(0x0, 0x0, 0x0, 100));
				sf2d_draw_texture_part_scale(spritesSmall, 227, 27 - ofs, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, 2, 2);
			}
			
			if (getGender(pkmn) == 0)
				sf2d_draw_texture(male, 159, 6);
			else if (getGender(pkmn) == 1)
				sf2d_draw_texture(female, 161, 6);
			
			for (int i = 0; i < 3; i++) {
				sf2d_draw_texture(button, 206, 110 + i * 27 + i*4);
				sftd_draw_text(fontBold12, 206 + (109 - sftd_get_text_width(fontBold12, 12, options[i])) / 2, 117 + i * 27 + i*4, BLACK, 12, options[i]);
			}
			
			for (int i = 0; i < 9; i++)
				if (i == 8 && isEgg(pkmn))
					sftd_draw_text(fontBold12, 2, 29 + i * 20, LIGHTBLUE, 12, "Egg cycle:");
				else
					sftd_draw_text(fontBold12, 2, 29 + i * 20, LIGHTBLUE, 12, entries[i]);

			for (int i = 0; i < 7; i++)
				sf2d_draw_texture(setting, 180, 51 + i * 20);

			sf2d_draw_texture(minus, 180 - max - 6 - minus->width, 29);
			sf2d_draw_texture(plus, 180, 29);
			sf2d_draw_texture(minus, 180 - max - 6 - minus->width, 189);
			sf2d_draw_texture(plus, 180, 189);

			char* level = (char*)malloc(4 * sizeof(char));
			snprintf(level, 4, "%u", getLevel(pkmn));
			sftd_draw_text(fontBold12, 180 - max - 3 + (max - sftd_get_text_width(fontBold12, 12, level)) / 2, 29, WHITE, 12, level);
			free(level);
				
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, natures[getNature(pkmn)]), 49, WHITE, 12, natures[getNature(pkmn)]);
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, abilities[getAbility(pkmn)]), 69, WHITE, 12, abilities[getAbility(pkmn)]);
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, items[getItem(pkmn)]), 89, WHITE, 12, items[getItem(pkmn)]);
			sftd_draw_text(fontBold12, 178 - sftd_get_text_width(fontBold12, 12, isShiny(pkmn) ? "Yes" : "No"), 109, WHITE, 12, isShiny(pkmn) ? "Yes" : "No");
			sftd_draw_text(fontBold12, 178 - sftd_get_text_width(fontBold12, 12, isInfected(pkmn) ? "Yes" : "No"), 129, WHITE, 12, isInfected(pkmn) ? "Yes" : "No");
			
			char* friendship = (char*)malloc(4 * sizeof(char));
			if (isEgg(pkmn))
				snprintf(friendship, 4, "%u", getOTFriendship(pkmn));
			else
				snprintf(friendship, 4, "%u", getFriendship(pkmn));
			sftd_draw_text(fontBold12, 180 - max - 3 + (max - sftd_get_text_width(fontBold12, 12, friendship)) / 2, 189, WHITE, 12, friendship);
			free(friendship);
			
			u32 nick[NICKNAMELENGTH*2];
			memset(nick, 0, NICKNAMELENGTH*2);
			getNickname(pkmn, nick);
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 178 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)nick)), 169, WHITE, 12, (wchar_t*)nick);

			u32 ot_name[NICKNAMELENGTH*2];
			memset(ot_name, 0, NICKNAMELENGTH*2);
			getOT(pkmn, ot_name);
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 178 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)ot_name)), 149, WHITE, 12, (wchar_t*)ot_name);
		}
		if (mode == ED_STATS || mode == ED_HIDDENPOWER) {
			sf2d_draw_texture(editorStatsBG, 0, 1);
			sf2d_draw_texture(editorBar, 0, 210);
			sf2d_draw_texture(setting, 291, 175);
			sftd_draw_text(fontBold12, 2, 28, LIGHTBLUE, 12, "STATS");
			sftd_draw_text(fontBold12, 118, 28, DARKBLUE, 12, "IV");
			sftd_draw_text(fontBold12, 197, 28, DARKBLUE, 12, "EV");
			sftd_draw_text(fontBold12, 256, 28, DARKBLUE, 12, "TOTAL");
			sftd_draw_text(fontBold12, 2, 173, LIGHTBLUE, 12, "Hidden Power:");
			
			sftd_draw_wtext(fontBold12, 27, 4, WHITE, 12, listSpecies.items[n]);
			sf2d_draw_texture_part(balls, -2, -6, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
			
			if (getGender(pkmn) == 0)
				sf2d_draw_texture(male, 159, 6);
			else if (getGender(pkmn) == 1)
				sf2d_draw_texture(female, 161, 6);
			
			for (int i = 0; i < 6; i++)
				sftd_draw_text(fontBold12, 2, 49 + i * 20, LIGHTBLUE, 12, values[i]);

			char* tmp = (char*)malloc(4);
			for (int i = 0; i < 6; i++) {
				snprintf(tmp, 4, "%d", getIV(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 112 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
				snprintf(tmp, 4, "%d", getEV(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 192 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
				snprintf(tmp, 4, "%d", getStat(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 263 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
			}
			free(tmp);
			
			sftd_draw_wtext(fontBold12, 288 - sftd_get_wtext_width(fontBold12, 12, hpList[getHPType(pkmn)]), 173, WHITE, 12, hpList[getHPType(pkmn)]);

			for (int i = 0; i < 6; i++) {
				sf2d_draw_texture(minus, 96, 49 + i * 20);
				sf2d_draw_texture(plus, 139, 49 + i * 20);
				sf2d_draw_texture(minus, 177, 49 + i * 20);
				sf2d_draw_texture(plus, 218, 49 + i * 20);
			}
		}
		if (mode == ED_MOVES) {

			sf2d_draw_texture(movesBottom, 0, 1);
			sf2d_draw_texture(movesBottom, 0, 2 + movesBottom->height);
			sf2d_draw_texture(editorBar, 0, 210);
			sftd_draw_text(fontBold12, 2, 5, LIGHTBLUE, 12, "Moves");
			sftd_draw_text(fontBold12, 2, 110, LIGHTBLUE, 12, "Relearn Moves");
			
			for (int i = 0; i < 4; i++) {
				sftd_draw_wtext(fontBold12, 2, 28 + i * 20, (i == additional3) ? YELLOW : WHITE, 12, moves[getMove(pkmn, i)]);
				sftd_draw_wtext(fontBold12, 2, 132 + i * 20, (i == additional3 - 4) ? YELLOW: WHITE, 12, moves[getEggMove(pkmn, i)]);
				if (i == additional3)
					sf2d_draw_texture_rotate(subArrow, 198 - movementOffsetSlow(3), 33 + i * 20, 3.1415f);
				else if (i == additional3 - 4)
					sf2d_draw_texture_rotate(subArrow, 198 - movementOffsetSlow(3), 137 + i * 20, 3.1415f);
			}

		}
		if (mode == ED_HEX) {
			printMenuBottom();
			sf2d_draw_texture(blueTextBox, 165, 28);
			if (sector[additional1][0] && !(sector[additional1][1])) {
				sf2d_draw_texture(minusButton, 224, 31);
				sf2d_draw_texture(plusButton, 247, 31);
			}
			if (hax)
				sf2d_draw_rectangle(0, 0, 320, 240, RGBA8(255, 0, 0, 100));
			
			sftd_draw_textf(fontBold14, 50, 30, LIGHTBLUE, 14, "Selected byte:");
			sftd_draw_textf(fontBold14, 171, 30, WHITE, 14, "0x%02hhX", additional1);
			
			printfHexEditorInfo(pkmn, additional1);
		}

		if (mode != ED_HEX)
			sf2d_draw_texture(back, 283, 211);
		
		// apply masks
		if (mode == ED_ITEMS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select an item with A in the top screen.")) / 2, 105, WHITE, 14, "Select an item with A in the top screen.");
		} else if (mode == ED_NATURES) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a nature with A in the top screen.")) / 2, 105, WHITE, 14, "Select a nature with A in the top screen.");
		} else if (mode == ED_BALLS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a ball with A in the top screen.")) / 2, 105, WHITE, 14, "Select a ball with A in the top screen.");
		} else if (mode == ED_HIDDENPOWER) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a HP type with A in the top screen.")) / 2, 105, WHITE, 14, "Select a HP type with A in the top screen.");
		} else if (mode == ED_FORMS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a form with A in the top screen.")) / 2, 105, WHITE, 14, "Select a form with A in the top screen.");
		}
		
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printPKBank(u8* bankbuf, u8* mainbuf, u8* pkmnbuf, int game, int currentEntry, int saveBox, int bankBox, bool isBufferized, bool isSeen) {
	int x, y;
	int pointer[2] = {0, 0};
	char* page = (char*)malloc(10 * sizeof(char));
	bool isKor = (pkmnbuf[0xE3] == 0x08) ? true : false;
	
	u8* pkmn = (u8*)malloc(PKMNLENGTH * sizeof(u8));
	if (currentEntry < 30)
		memcpy(pkmn, &bankbuf[bankBox * 30 * PKMNLENGTH + currentEntry * PKMNLENGTH], PKMNLENGTH);
	else
		getPkmn(mainbuf, saveBox, currentEntry - 30, pkmn, game);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		if (isSeen) {
			infoViewer(pkmnbuf, game);
		} else {
			printAnimatedBG(true);
			sf2d_draw_texture(bankTop, 34, 5);
			snprintf(page, 10, "Bank %d", bankBox + 1);
			sftd_draw_text(fontBold12, 55 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 9, WHITE, 12, page);

			if (getPokedexNumber(pkmn) > 0 && getPokedexNumber(pkmn) < 822) {
				u16 tempspecies = getFormSpeciesNumber(pkmn);
				u8 type1 = 0, type2 = 0;

				if (tempspecies == 493 || tempspecies == 773) {
					type1 = getForm(pkmn);
					if (game >= GAME_DIAMOND && game <= GAME_SS && type1 > 9)
						type1--;
					type2 = type1;
				} else {
					type1 = personal.pkmData[tempspecies][0x6];
					type2 = personal.pkmData[tempspecies][0x7];
				}
				
				sf2d_draw_texture_part(typesSheet, 273, 120, 50 * type1, 0, 50, 18); 
				if (type1 != type2)
					sf2d_draw_texture_part(typesSheet, 325, 120, 50 * type2, 0, 50, 18); 
				
				u32 nick[NICKNAMELENGTH*2];
				memset(nick, 0, NICKNAMELENGTH*2);
				getNickname(pkmn, nick);
				sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 273, 69, WHITE, 12, (wchar_t*)nick);
				
				char* level = (char*)malloc(8 * sizeof(char));
				snprintf(level, 8, "Lv.%u", getLevel(pkmn));
				sftd_draw_text(fontBold12, 372 - sftd_get_text_width(fontBold12, 12, level), 86, WHITE, 12, level);
				
				if (getGender(pkmn) == 0)
					sf2d_draw_texture(male, 358 - sftd_get_text_width(fontBold12, 12, level), 86);
				else if (getGender(pkmn) == 1)
					sf2d_draw_texture(female, 360 - sftd_get_text_width(fontBold12, 12, level), 86);
				if (isShiny(pkmn))
					sf2d_draw_texture(shinyStar, 360 - sftd_get_text_width(fontBold12, 12, level) - 14, 88);
				
				free(level);
				
				u32 ot_name[NICKNAMELENGTH*2];
				memset(ot_name, 0, NICKNAMELENGTH*2);
				getOT(pkmn, ot_name);
				sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 273, 146, WHITE, 12, (wchar_t*)ot_name);

				char* otid = (char*)malloc(12 * sizeof(char));
				snprintf(otid, 12, "ID. %u", getOTID(pkmn));
				sftd_draw_text(fontBold12, 372 - sftd_get_text_width(fontBold12, 12, otid), 163, WHITE, 12, otid);
				free(otid);
				
				sftd_draw_wtext(fontBold12, 273, 104, WHITE, 12, listSpecies.items[getPokedexNumber(pkmn)]);
			}
			y = 45;
			for (int i = 0; i < 5; i++) {
				x = 44;
				for (int j = 0; j < 6; j++) {
					memcpy(pkmn, &bankbuf[bankBox * 30 * PKMNLENGTH + (i * 6 + j) * PKMNLENGTH], PKMNLENGTH);
					u16 n = getPokedexNumber(pkmn);
					if (n)
						printElement(pkmn, GAME_SUN, n, x, y);

					if (currentEntry == (i * 6 + j)) {
						pointer[0] = x + 18;
						pointer[1] = y - 8;
					}
					x += 34;
				}
				y += 30;
			}
			
			if (currentEntry < 30) {
				u16 n = getPokedexNumber(pkmnbuf);
				if (n) printElementBlend(pkmnbuf, GAME_SUN, n, pointer[0] - 14, pointer[1] + 8, RGBA8(0x0, 0x0, 0x0, 100));
				if (n) printElement(pkmnbuf, GAME_SUN, n, pointer[0] - 18, pointer[1] + 3);
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2 - ((!isBufferized) ? movementOffsetSlow(3) : 0));
			}
		}		
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(boxView, 0, 0);
		sf2d_draw_texture(editorBar, 0, 210);
		snprintf(page, 10, "Save %d", saveBox + 1);
		sftd_draw_text(fontBold12, 12 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 19, WHITE, 12, page);
		sf2d_draw_texture(left, 7, 17);
		sf2d_draw_texture(right, 185, 17);
		sf2d_draw_texture(transferButton, 242, 5);
		sf2d_draw_texture(back, 283, 211);
		sf2d_draw_texture(button, 208, 43);
		sf2d_draw_texture(button, 208, 70);
		sf2d_draw_texture(button, 208, 97);
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "Y: VIEW")) / 2, 50, BLACK, 12, "Y: VIEW");
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "CLEAR BOX")) / 2, 77, BLACK, 12, "CLEAR BOX");
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "RELEASE")) / 2, 104, BLACK, 12, "RELEASE");
				
		y = 45;
		for (int i = 0; i < 5; i++) {
			x = 4;
			for (int j = 0; j < 6; j++) {
				getPkmn(mainbuf, saveBox, i*6+j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n)
					printElement(pkmn, game, n, x, y);

				if ((currentEntry - 30) == (i * 6 + j)) {
					pointer[0] = x + 18;
					pointer[1] = y - 8;
				}
				x += 34;
			}
			y += 30;
		}
		
		if (currentEntry > 29) {
			if (!isSeen) {
				u16 n = getPokedexNumber(pkmnbuf);
				if (n) printElementBlend(pkmnbuf, GAME_SUN, n, pointer[0] - 14, pointer[1] + 8, RGBA8(0x0, 0x0, 0x0, 100));
				if (n) printElement(pkmnbuf, GAME_SUN, n, pointer[0] - 18, pointer[1] + 3);
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2 - ((!isBufferized) ? movementOffsetSlow(3) : 0));
			} else
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2);
		}
		
		if (isSeen)
			sf2d_draw_rectangle(0, -30, 320, 240, MASKBLACK);
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pkmn);
	free(page);
}

void printMassInjector(int currentEntry) {
	int y = 41;
	char* entries[] = {"XD Collection", "Colosseum Collection", "10th Anniversary Collection", "N's Collection", "Entree Forest Collection", "Dream Radar Collection", "Living Dex", "Oblivia Deoxys Collection", "Pokemon Ranch Collection", "KOR Events Collection"};
	int boxes[] = {3, 2, 1, 1, 1, 1, 27, 1, 1, 1};
	
	char* message = (char*)malloc(30 * sizeof(char));
	snprintf(message, 30, "%d boxes will be replaced.", boxes[currentEntry]);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printMenuTop();
		printTitle("Mass Injector");
		
		for (int i = 0; i < 5; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			sftd_draw_text(fontBold12, 18 + (182 - sftd_get_text_width(fontBold12, 12, entries[i])) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, entries[i]);
			
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			sftd_draw_text(fontBold12, 200 + (182 - sftd_get_text_width(fontBold12, 12, entries[i])) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, entries[i]);
			
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();
		sftd_draw_text(fontBold12, (320 - sftd_get_text_width(fontBold12, 12, message)) / 2, 12, LIGHTBLUE, 12, message);
		printBottomIndications("Press START to inject, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(message);
}

void printSettings(int box) {
	char *menu[] = {"Bank Size:                   ", "Backup Save", "Backup Bank"};
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		printMenuBottom();

		for (int i = 0; i < 3; i++) {
			sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 60 + i * (menuBar->height));
			sftd_draw_text(fontBold15, (320 - sftd_get_text_width(fontBold15, 15, menu[i])) / 2, 58 + (menuBar->height - 18) / 2 + i * (menuBar->height), DARKBLUE, 15, menu[i]);
		}
		
		sf2d_draw_texture(miniBox, 189, 64);
		sf2d_draw_texture(minusButton, 169, 65);
		sf2d_draw_texture(plusButton, 228, 65);
		
		char* size = (char*)malloc(5);
		snprintf(size, 5, "%d", box);
		sftd_draw_text(fontBold11, 189 + (36 - (sftd_get_text_width(fontBold11, 11, size))) / 2, 68, WHITE, 11, size);
		free(size);

		sftd_draw_text(fontBold9, 85, 225, LIGHTBLUE, 9, "Tap the number to change size.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printfHexEditorInfo(u8* pkmn, int byte) {
	int y = 70, x = 8, xribbon = 90;
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	u32 string[NICKNAMELENGTH*2];
	memset(string, 0, NICKNAMELENGTH*2);
	
	switch (byte) {
		case 0x08 :
		case 0x09 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Species: #%d - %ls", getPokedexNumber(pkmn), listSpecies.items[getPokedexNumber(pkmn)]);
			break;
		case 0x0A :
		case 0x0B :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Held Item: #%d - %ls", getItem(pkmn), items[getItem(pkmn)]);
			break;
		case 0x0C :
		case 0x0D :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "TID: %d", getOTID(pkmn));
			break;
		case 0x0E :
		case 0x0F :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SID: %d", getSOTID(pkmn));
			break;
		case 0x14 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Ability: %ls", abilities[getAbility(pkmn)]);
			break;
		case 0x1C :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Nature: %ls", natures[getNature(pkmn)]);
			break;
		case 0x1D : {
			char* entries[] = { "Fateful Encounter flag" };
			int i = sftd_get_text_width(fontBold12, 12, "Gender: ");
			sftd_draw_text(fontBold12, xribbon + 27, y, LIGHTBLUE, 12, entries[0]);
			sf2d_draw_rectangle(xribbon, y, 13, 13, (pkmn[byte] & 1) ? BUTTONGREEN : BUTTONRED);
			sftd_draw_text(fontBold12, x, y + 17, LIGHTBLUE, 12, "Gender: ");
			if (getGender(pkmn) == 0)
				sftd_draw_text(fontBold12, x + i, y + 17, LIGHTBLUE, 12, "male");
			else if (getGender(pkmn) == 1)
				sftd_draw_text(fontBold12, x + i, y + 17, LIGHTBLUE, 12, "female");
			else
				sftd_draw_text(fontBold12, x + i, y + 17, LIGHTBLUE, 12, "genderless");
			sftd_draw_textf(fontBold12, x, y + 17 * 2, LIGHTBLUE, 12, "Alternative Form: #%d", pkmn[byte] >> 3);
			break;
		}
		case 0x1E :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "HP EV: %d", pkmn[byte]);
			break;
		case 0x1F : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "ATK EV: %d", pkmn[byte]);
			break;
		case 0x20 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "DEF EV: %d", pkmn[byte]);
			break;	
		case 0x21 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPE EV: %d", pkmn[byte]);
			break;
		case 0x22 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPA EV: %d", pkmn[byte]);
			break;	
		case 0x23 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPD EV: %d", pkmn[byte]);
			break;	
		case 0x24 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Cool: %d", pkmn[byte]);
			break;	
		case 0x25 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Beauty: %d", pkmn[byte]);
			break;	
		case 0x26 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Cute: %d", pkmn[byte]);
			break;	
		case 0x27 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Smart: %d", pkmn[byte]);
			break;	
		case 0x28 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Tough: %d", pkmn[byte]);
			break;	
		case 0x29 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Sheen: %d", pkmn[byte]);
			break;
		case 0x30 : {
			char* entries[] = {"Champion Kalos", "Champion G3 Hoenn", "Champion Sinnoh", "Best Friends", "Training", "Skillful Battler", "Battler Expert", "Effort"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 0, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x31 : {
			char* entries[] = {"Alert", "Shock", "Downcast", "Careless", "Relax", "Snooze", "Smile", "Gorgeous"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 1, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x32 : {
			char* entries[] = {"Royal", "Gorgeous Royal", "Artist", "Footprint", "Record", "Legend", "Country", "National"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 2, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x33 : {
			char* entries[] = {"Earth", "World", "Classic", "Premier", "Event", "Birthday", "Special", "Souvenir"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 3, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x34 : {
			char* entries[] = {"Wishing", "Champion Battle", "Champion Regional", "Champion National", "Champion World", "-", "-", "Champion G6 Hoenn"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 4, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x35 : {
			char* entries[] = {"Contest Star", "Master Coolness", "Master Beauty", "Master Cuteness", "Master Cleverness", "Master Toughness", "Champion Alola", "Battle Royale"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 5, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x36 : {
			char* entries[] = {"Battle Tree Great", "Battle Tree Master", "-", "-", "-", "-", "-", "-"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 6, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x40 :
		case 0x41 :
		case 0x42 :
		case 0x43 :
		case 0x44 :
		case 0x45 :
		case 0x46 :
		case 0x47 :
		case 0x48 :
		case 0x49 :
		case 0x4A :
		case 0x4B :
		case 0x4C :
		case 0x4D :
		case 0x4E :
		case 0x4F :
		case 0x50 :
		case 0x51 :
		case 0x52 :
		case 0x53 :
		case 0x54 :
		case 0x55 :
		case 0x56 :
		case 0x57 :
			getNickname(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Nickname: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_text_width(fontBold12, 12, "Nickname: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0x5A :
		case 0x5B :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 1: #%d - %ls", getMove(pkmn, 0), moves[getMove(pkmn, 0)]);
			break;
		case 0x5C :
		case 0x5D :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 2: #%d - %ls", getMove(pkmn, 1), moves[getMove(pkmn, 1)]);
			break;
		case 0x5E :
		case 0x5F :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 3: #%d - %ls", getMove(pkmn, 2), moves[getMove(pkmn, 2)]);
			break;
		case 0x60 :
		case 0x61 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 4: #%d - %ls", getMove(pkmn, 3), moves[getMove(pkmn, 3)]);
			break;
		case 0x62 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 1 PP: %d", pkmn[byte]);
			break;
		case 0x63 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 2 PP: %d", pkmn[byte]);
			break;
		case 0x64 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 3 PP: %d", pkmn[byte]);
			break;
		case 0x65 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 4 PP: %d", pkmn[byte]);
			break;
		case 0x66 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 1 PP Up: %d", pkmn[byte]);
			break;
		case 0x67 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 2 PP Up: %d", pkmn[byte]);
			break;
		case 0x68 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 3 PP Up: %d", pkmn[byte]);
			break;
		case 0x69 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 4 PP Up: %d", pkmn[byte]);
			break;
		case 0x6A :
		case 0x6B :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Relearn Move 1: #%d - %ls", getEggMove(pkmn, 0), moves[getEggMove(pkmn, 0)]);
			break;
		case 0x6C :
		case 0x6D :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Relearn Move 2: #%d - %ls", getEggMove(pkmn, 1), moves[getEggMove(pkmn, 1)]);
			break;
		case 0x6E :
		case 0x6F :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Relearn Move 3: #%d - %ls", getEggMove(pkmn, 2), moves[getEggMove(pkmn, 2)]);
			break;
		case 0x70 :
		case 0x71 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Relearn Move 4: #%d - %ls", getEggMove(pkmn, 3), moves[getEggMove(pkmn, 3)]);
			break;
		case 0x77: {
			char* entries[] = { "is Nicknamed", "is Egg" };
			sftd_draw_text(fontBold12, xribbon + 27, y, LIGHTBLUE, 12, entries[0]);
			sf2d_draw_rectangle(xribbon, y, 13, 13, (isNicknameF(pkmn)) ? BUTTONGREEN : BUTTONRED);
			sftd_draw_text(fontBold12, xribbon + 27, y + 17, LIGHTBLUE, 12, entries[1]);
			sf2d_draw_rectangle(xribbon, y + 17, 13, 13, (isEgg(pkmn)) ? BUTTONGREEN : BUTTONRED);
			break;
		}
		case 0x78 :
		case 0x79 :
		case 0x7A :
		case 0x7B :
		case 0x7C :
		case 0x7D :
		case 0x7E :
		case 0x7F :
		case 0x80 :
		case 0x81 :
		case 0x82 :
		case 0x83 :
		case 0x84 :
		case 0x85 :
		case 0x86 :
		case 0x87 :
		case 0x88 :
		case 0x89 :
		case 0x8A :
		case 0x8B :	
		case 0x8C :
		case 0x8D :
		case 0x8E :
		case 0x8F :	
			getHTName(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer name: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_text_width(fontBold12, 12, "Held Trainer name: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0xA2 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Friendship: %d", pkmn[byte]);
			break;
		case 0xA3 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Affection: %d", pkmn[byte]);
			break;
		case 0xA4 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Intensity: %d", pkmn[byte]);
			break;
		case 0xA5 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Memory: %d", pkmn[byte]);
			break;	
		case 0xA6 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Feeling: %d", pkmn[byte]);
			break;
		case 0xAE :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Fullness: %d", pkmn[byte]);
			break;	
		case 0xAF :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Enjoyment: %d", pkmn[byte]);
			break;
		case 0xB0 :
		case 0xB1 :
		case 0xB2 :
		case 0xB3 :
		case 0xB4 :
		case 0xB5 :
		case 0xB6 :
		case 0xB7 :
		case 0xB8 :
		case 0xB9 :
		case 0xBA :
		case 0xBB :
		case 0xBC :
		case 0xBD :
		case 0xBE :
		case 0xBF :
		case 0xC0 :
		case 0xC1 :
		case 0xC2 :
		case 0xC3 :
		case 0xC4 :
		case 0xC5 :
		case 0xC6 :
		case 0xC7 :
			getOT(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Name: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_text_width(fontBold12, 12, "Original Trainer Name: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0xCA :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Friendship: %d", pkmn[byte]);
			break;
		case 0xCB :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Affection: %d", pkmn[byte]);
			break;
		case 0xCC :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Intensity: %d", pkmn[byte]);
			break;
		case 0xCD :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Memory: %d", pkmn[byte]);
			break;	
		case 0xD0 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Feeling: %d", pkmn[byte]);
			break;
		case 0xD1 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Year: %d", 2000 + pkmn[byte]);
			break;
		case 0xD2 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Month: %d", pkmn[byte]);
			break;
		case 0xD3 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Day: %d", pkmn[byte]);
			break;
		case 0xD4 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Year: %d", 2000 + pkmn[byte]);
			break;
		case 0xD5 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Month: %d", pkmn[byte]);
			break;
		case 0xD6 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Day: %d", pkmn[byte]);
			break;
		case 0xDD : {
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Level: %d", (pkmn[byte] & 0x7f));
			sftd_draw_textf(fontBold12, xribbon + 27, y + 17, LIGHTBLUE, 12, ": Original Trainer Gender");
			if (getOTGender(pkmn) == 0)
				sf2d_draw_texture(male, xribbon + 10, y + 17 + 2);
			else if (getOTGender(pkmn) == 1)
				sf2d_draw_texture(female, xribbon + 10 + 2, y + 17 + 2);
			break;
		}
		case 0xDE : {
			char* entries[] = { "HP Hyper Trained", "ATK Hyper Trained", "DEF Hyper Trained", "SP.ATK Hyper Trained", "SP.DEF Hyper Trained", "SPEED Hyper Trained" };
			for (int i = 0; i < 6; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17 * i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17 * i, 13, 13, (getHTi(pkmn, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
	}
}
