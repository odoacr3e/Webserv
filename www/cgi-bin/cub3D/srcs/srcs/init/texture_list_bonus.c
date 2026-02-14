/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_list_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:27:55 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 08:20:59 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

char *g_textures[TEXTURES_NUM] = {
	[SCREEN] = SCREEN_data,
	[NORTH] = NORTH_data,
	[EAST] = EAST_data,
	[SOUTH] = SOUTH_data,
	[WEST] = WEST_data,
	[CROSSHAIR] = CROSSHAIR_data,
	[PLAYER] = BARBARIAN_CHILL_data,
	[WALL] = DEBUG_WALL_data,
	[DOOR_OPEN] = DOOR_OPEN_data,
	[DOOR_CLOSE] = DOOR_CLOSED_data,
	[M_BACKGROUND] = MINI_BACKGROUND_data,
	[M_PLAYER_0] = MINI_PLAYER_0_data,
	[M_PLAYER_20] = MINI_PLAYER_20_data,
	[M_PLAYER_40] = MINI_PLAYER_40_data,
	[M_PLAYER_60] = MINI_PLAYER_60_data,
	[M_PLAYER_80] = MINI_PLAYER_80_data,
	[M_PLAYER_100] = MINI_PLAYER_100_data,
	[M_PLAYER_120] = MINI_PLAYER_120_data,
	[M_PLAYER_140] = MINI_PLAYER_140_data,
	[M_PLAYER_160] = MINI_PLAYER_160_data,
	[M_PLAYER_180] = MINI_PLAYER_180_data,
	[M_PLAYER_200] = MINI_PLAYER_200_data,
	[M_PLAYER_220] = MINI_PLAYER_220_data,
	[M_PLAYER_240] = MINI_PLAYER_240_data,
	[M_PLAYER_260] = MINI_PLAYER_260_data,
	[M_PLAYER_280] = MINI_PLAYER_280_data,
	[M_PLAYER_300] = MINI_PLAYER_300_data,
	[M_PLAYER_320] = MINI_PLAYER_320_data,
	[M_PLAYER_340] = MINI_PLAYER_340_data,
	[M_DOOR] = MINI_DOOR_data,
	[M_COIN] = MINI_COIN_data,
	[M_FOE] = MINI_FOE_data,
	[FOE1] = FOE_1_data,
	[FOE2] = FOE_2_data,
	[FOE3] = FOE_3_data,
	[FOE4] = FOE_4_data,
	[FOE5] = FOE_5_data,
	[FOE6] = FOE_6_data,
	[FOE7] = FOE_7_data,
	[FOE8] = FOE_8_data,
	[FOE9] = FOE_9_data,
	[FOE10] = FOE_10_data,
	[FOE11] = FOE_11_data,
	[FOE12] = FOE_12_data,
	[FOE_BIT1] = BARBARIAN_1LEFT_data,
	[FOE_BIT2] = BARBARIAN_2LEFT_data,
	[FOE_BIT3] = BARBARIAN_1RIGHT_data,
	[FOE_BIT4] = BARBARIAN_2RIGHT_data,
	[BARBARIAN_CHILL] = BARBARIAN_CHILL_data,
	[BARBARIAN_ROCK] = BARBARIAN_ROCK_data,
	[BARBARIAN_PAPER] = BARBARIAN_PAPER_data,
	[BARBARIAN_SCISSOR] = BARBARIAN_SCISSORS_data,
	[BARBARIAN_SCARED] = BARBARIAN_SCARED_data,
	[PHILIP_CHILL] = PHILIP_CHILL_data,
	[PHILIP_ROCK] = PHILIP_ROCK_data,
	[PHILIP_PAPER] = PHILIP_PAPER_data,
	[PHILIP_SCISSOR] = PHILIP_SCISSORS_data,
	[PHILIP_LOOK] = PHILIP_LOOK_data,
	[COIN] = COIN_data,
	[ARMS1] = POSSENZA_data,
	[ARMS2] = POSSENZA2_data,
	[BAGUETTE] = BAGUETTE_data,
	[MENU_FRAME] = MENU_IL_DESTINO_data,
	[PLAY_BUTTON] = PLAY_BUTTON_data,
	[EXIT_BUTTON] = EXIT_BUTTON_data,
	[BATTLE_FRAME] = HOLY_FIGHT_SCENARIO_data,
	[LOSS_MSG] = LOSS_MSG_data,
	[WIN_MSG] = WIN_MSG_data,
	[DRAW_MSG] = DRAW_MSG_data,
	[DEATH1] = DEATH1_data,
	[DEATH2] = DEATH2_data,
	[DEATH3] = DEATH3_data,
	[DEATH4] = DEATH4_data,
	[DEATH5] = DEATH5_data,
	[DEATH6] = DEATH6_data,
	[DEATH7] = DEATH7_data,
	[ROCK] = ROCK_data,
	[PAPER] = PAPER_data,
	[SCISSORS] = SCISSORS_data
};

unsigned char bmp_header[54] = {
	// --- BITMAP FILE HEADER (14 bytes) ---
	0x42, 0x4D,             // "BM" MAGIC NUMBER
	0x36, 0x00, 0x96, 0x00, // Dimensione totale file (9.830.454 bytes)
	0x00, 0x00, 0x00, 0x00, // Riservato
	0x36, 0x00, 0x00, 0x00, // Offset inizio dati pixel (54)

	// --- DIB HEADER (BITMAPINFOHEADER - 40 bytes) ---
	0x28, 0x00, 0x00, 0x00, // Dimensione di questo header (40)
	0x80, 0x07, 0x00, 0x00, // Larghezza: 1920
	0x00, 0xFB, 0xFF, 0xFF, // Altezza: -1280 (Top-Down, Complemento a due)
	0x01, 0x00,             // Piani (sempre 1)
	0x20, 0x00,             // Bit per Pixel (32)
	0x00, 0x00, 0x00, 0x00, // Compressione (0 = BI_RGB, nessuna)
	0x00, 0x00, 0x96, 0x00, // Dimensione dati immagine (9.830.400 bytes)
	0x13, 0x0B, 0x00, 0x00, // Ris. Orizzontale (2835 pixel/m)
	0x13, 0x0B, 0x00, 0x00, // Ris. Verticale (2835 pixel/m)
	0x00, 0x00, 0x00, 0x00, // Colori usati (0 = tutti)
	0x00, 0x00, 0x00, 0x00  // Colori importanti (0 = tutti)
};
