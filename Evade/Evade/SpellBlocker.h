#ifndef SpellBlocker_h__
#define SpellBlocker_h__

#include "PluginSDK.h"

namespace SpellBlocker
{
	extern std::vector<int> Spells;

	static void LoadSpells()
	{
		std::string szChamp = std::string(GEntityList->Player()->ChampionName());
		size_t iHash = std::hash<std::string>{}(szChamp);

		switch (iHash)
		{
		case 0x717CDBE6: /* Aatrox */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0x21C93469: /* Ahri */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0xD65AF7AB: /* Akali */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x77FDD87: /* Alistar */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x7A20D7A8: /* Amumu */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x14F4222B: /* Anivia */
			Spells = std::vector<int>({ kSlotQ, kSlotW }); 
			break;
		case 0x27E47192: /* Annie */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0xA9DEE00E: /* Ashe */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0xB18A1139: /* Azir */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0xC7BA247A: /* Bard */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x4B733DE7: /* Blitzcrank */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x6D1C7E96: /* Brand */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0x56069412: /* Braum */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x4E0475FF: /* Caitlyn */
			Spells = std::vector<int>({ kSlotE }); 
			break;
		case 0x786A8F36: /* Cassiopeia */
			Spells = std::vector<int>(); 
			break;
		case 0xA85DD473: /* Chogath */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x143F059F: /* Corki */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x98F67577: /* Darius */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0xC80FC558: /* Diana */
			Spells = std::vector<int>({ kSlotW, kSlotR });
			break;
		case 0xB4012FE3: /* Draven */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x69D7F548: /* DrMundo */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0xA23DA093: /* Ekko */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0xB202D32B: /* Elise */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xF7550FF2: /* Evelynn */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0xBA148E1E: /* Ezreal */
			Spells = std::vector<int>({ kSlotE }); 
			break;
		case 0x81F76A62: /* Fiddlesticks */
			Spells = std::vector<int>({ kSlotQ }); 
			break;
		case 0x32C8BEA4: /* Fiora */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x27EB120C: /* Fizz */
			Spells = std::vector<int>({ kSlotW, kSlotW, kSlotE }); 
			break;
		case 0xD698F90B: /* Galio */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0xA91550A6: /* Gangplank */
			Spells = std::vector<int>({ kSlotW, kSlotE });
			break;
		case 0xEA40C21A: /* Garen */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0x1E5886EB: /* Gnar */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xA1D8FAC0: /* Gragas */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xDBF24173: /* Graves */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0xF0619CDA: /* Hecarim */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x63D726AC: /* Heimerdinger */
			Spells = std::vector<int>({ kSlotR }); 
			break;
		case 0x4A5C5DE5: /* Irelia */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xCE9E1BF3: /* Janna */
			Spells = std::vector<int>({ kSlotQ, kSlotE }); 
			break;
		case 0xD3333F20: /* JarvanIV */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x38F2B8A4: /* Jax */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x7B02A21B: /* Jayce */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x1F7C118A: /* Jinx */
			Spells = std::vector<int>({ kSlotQ, kSlotE }); 
			break;
		case 0x561FFB74: /* Kalista */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0x61065D67: /* Karma */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xF60F4381: /* Karthus */
			Spells = std::vector<int>({ kSlotE }); 
			break;
		case 0x13FDAD15: /* Kassadin */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x7E835DC2: /* Katarina */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x7E2950FB: /* Kayle */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x9B78CD0A: /* Kennen */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xBBA9E6BC: /* Khazix */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0x1AF63217: /* KogMaw */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0x20208B18: /* Leblanc */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0xF9552CF5: /* LeeSin */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x6CBDE4C6: /* Leona */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0xE510E22C: /* Lissandra */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0x873F5281: /* Lucian */
			Spells = std::vector<int>({ kSlotE }); 
			break;
		case 0x65356775: /* Lulu */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xA71C9826: /* Lux */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x9F74F37F: /* Malphite */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x6B5D5567: /* Malzahar */
			Spells = std::vector<int>({ kSlotE }); 
			break;
		case 0xC4D87FCF: /* Maokai */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xD7471F75: /* MasterYi */
			Spells = std::vector<int>({ kSlotQ, kSlotE, kSlotR }); 
			break;
		case 0x8B5F883A: /* MissFortune */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0xF6386407: /* MonkeyKing */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x631AA365: /* Mordekaiser */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0x1D0BEFF0: /* Morgana */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0x1BE085EA: /* Nami */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x63D2F515: /* Nasus */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0x42869618: /* Nautilus */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xBF512A33: /* Nidalee */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x1FEF72CF: /* Nocturne */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x4382969: /* Nunu */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0x599A49: /* Olaf */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xABF8AF0D: /* Orianna */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE }); 
			break;
		case 0xE692E47A: /* Pantheon */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0xD3E01E7B: /* Poppy */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x97FD7C42: /* Quinn */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xE3D8CFDA: /* Rammus */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0x31E78EF4: /* RekSai */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x424D58B: /* Renekton */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xAE16C500: /* Rengar */
			Spells = std::vector<int>({ kSlotQ, kSlotW }); 
			break;
		case 0xFA96C495: /* Riven */
			Spells = std::vector<int>({ kSlotQ, kSlotE, kSlotR }); 
			break;
		case 0xFD3D22D4: /* Rumble */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0xCBEBE84D: /* Ryze */
			Spells = std::vector<int>({ kSlotR }); 
			break;
		case 0x73BFF612: /* Sejuani */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE });
			break;
		case 0x495A835F: /* Shaco */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0x228DBB3D: /* Shen */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR });
			break;
		case 0x20143239: /* Shyvana */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR });
			break;
		case 0xCE8CF43F: /* Singed */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0xAF76FADC: /* Sion */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xE51ECE2: /* Sivir */
			Spells = std::vector<int>({ kSlotE, kSlotR }); 
			break;
		case 0xCDEA6D09: /* Skarner */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x8F8B560E: /* Sona */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xCC6AA066: /* Soraka */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xC96F05F5: /* Swain */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xA4382FDE: /* Syndra */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotR }); 
			break;
		case 0x2EA4EDA2: /* TahmKench */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x7010E237: /* Talon */
			Spells = std::vector<int>({ kSlotQ, kSlotE, kSlotR }); 
			break;
		case 0xBD5FE1A4: /* Taric */
			Spells = std::vector<int>({ kSlotR }); 
			break;
		case 0x52B2EC85: /* Teemo */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0xE26DAAB7: /* Thresh */
			Spells = std::vector<int>({ kSlotQ });
			break;
		case 0xD7285C89: /* Tristana */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0xDF2C7333: /* Trundle */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x53AA70F4: /* Tryndamere */
			Spells = std::vector<int>({ kSlotQ, kSlotE, kSlotR }); 
			break;
		case 0xDD7D9F77: /* TwistedFate */
			Spells = std::vector<int>({ kSlotW, kSlotE }); 
			break;
		case 0x8EB3C06A: /* Twitch */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0x2EF4E597: /* Udyr */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xE68AEF40: /* Urgot */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x89080C66: /* Varus */
			Spells = std::vector<int>(); 
			break;
		case 0xABD17D26: /* Vayne */
			Spells = std::vector<int>({ kSlotQ, kSlotR }); 
			break;
		case 0x525EE9E3: /* Veigar */
			Spells = std::vector<int>(); 
			break;
		case 0x3CEA1838: /* Velkoz */
			Spells = std::vector<int>({ kSlotQ, kSlotE }); 
			break;
		case 0x3BFA0A00: /* Vi */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x8CC76A46: /* Viktor */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xB5F08597: /* Vladimir */
			Spells = std::vector<int>({ kSlotW, kSlotR }); 
			break;
		case 0x8A0287EF: /* Volibear */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xCD0A733: /* Warwick */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x2051CDD3: /* Xerath */
			Spells = std::vector<int>(); 
			break;
		case 0x50313A26: /* XinZhao */
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x4718CF94: /* Yasuo */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xDD17CB76: /* Yorick */
			Spells = std::vector<int>(); 
			break;
		case 0xF1206565: /* Zac */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x1A161894: /* Zed */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0x4A0FF623: /* Ziggs */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		case 0x8F5670FC: /* Zilean */
			Spells = std::vector<int>({ kSlotW, kSlotE, kSlotR }); 
			break;
		case 0xD0A81D1: /* Zyra */
			Spells = std::vector<int>({ kSlotW }); 
			break;
		default:
			Spells = std::vector<int>({ kSlotQ, kSlotW, kSlotE, kSlotR }); 
			break;
		}
	}

	static bool CanBlock(int slot)
	{
		if (slot == kSummonerSlot1 || slot == kSummonerSlot2)
			return false;

		for (auto i : Spells)
		{
			if (i == slot)
				return false;
		}

		return (slot == kSlotQ || slot == kSlotW || slot == kSlotE || slot == kSlotR);
	}
}

#endif // SpellBlocker_h__
