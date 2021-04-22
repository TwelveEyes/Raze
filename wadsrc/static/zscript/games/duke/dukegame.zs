// contains all global Duke definitions
struct Duke native
{
	enum ESpecialMusic
	{
		MUS_INTRO = 0,
		MUS_BRIEFING = 1,
		MUS_LOADING = 2,
	};
	
	enum EPalette
	{
		BASEPAL = 0,
		WATERPAL,
		SLIMEPAL,
		TITLEPAL,
		DREALMSPAL,
		ENDINGPAL,  // 5
		ANIMPAL,    // not used anymore. The anim code now generates true color textures.
		DRUGPAL,
		BASEPALCOUNT
	};

	native static void PlaySpecialMusic(int which);
	native static int PlaySound(int num, int channel = CHAN_AUTO, int flags = 0, float vol =0.8f);

}

struct DukeSnd native
{
	// This really needs to be done better...
	enum EDukeSounds
	{
		KICK_HIT                   =  0,
		PISTOL_RICOCHET            =  1,
		PISTOL_BODYHIT             =  2,
		PISTOL_FIRE                =  3,
		EJECT_CLIP                 =  4,
		INSERT_CLIP                =  5,
		CHAINGUN_FIRE              =  6,
		RPG_SHOOT                  =  7,
		POOLBALLHIT                =  8,
		RPG_EXPLODE                =  9,
		CAT_FIRE                   = 10,
		SHRINKER_FIRE              = 11,
		ACTOR_SHRINKING            = 12,
		PIPEBOMB_BOUNCE            = 13,
		PIPEBOMB_EXPLODE           = 14,
		LASERTRIP_ONWALL           = 15,
		LASERTRIP_ARMING           = 16,
		LASERTRIP_EXPLODE          = 17,
		VENT_BUST                  = 18,
		GLASS_BREAKING             = 19,
		GLASS_HEAVYBREAK           = 20,
		SHORT_CIRCUIT              = 21,
		ITEM_SPLASH                = 22,
		DUKE_BREATHING             = 23,
		DUKE_EXHALING              = 24,
		DUKE_GASP                  = 25,
		SLIM_RECOG                 = 26,

		DUKE_URINATE               = 28,
		ENDSEQVOL3SND2             = 29,
		ENDSEQVOL3SND3             = 30,
		DUKE_PASSWIND              = 32,
		DUKE_CRACK                 = 33,
		SLIM_ATTACK                = 34,
		SOMETHINGHITFORCE          = 35,
		DUKE_DRINKING              = 36,
		DUKE_KILLED1               = 37,
		DUKE_GRUNT                 = 38,
		DUKE_HARTBEAT              = 39,
		DUKE_ONWATER               = 40,
		DUKE_DEAD                  = 41,
		DUKE_LAND                  = 42,
		DUKE_WALKINDUCTS           = 43,
		DUKE_GLAD                  = 44,
		DUKE_YES                   = 45,
		DUKE_HEHE                  = 46,
		DUKE_SHUCKS                = 47,
		DUKE_UNDERWATER            = 48,
		DUKE_JETPACK_ON            = 49,
		DUKE_JETPACK_IDLE          = 50,
		DUKE_JETPACK_OFF           = 51,
		LIZTROOP_GROWL             = 52,
		LIZTROOP_TALK1             = 53,
		LIZTROOP_TALK2             = 54,
		LIZTROOP_TALK3             = 55,
		DUKETALKTOBOSS             = 56,
		LIZCAPT_GROWL              = 57,
		LIZCAPT_TALK1              = 58,
		LIZCAPT_TALK2              = 59,
		LIZCAPT_TALK3              = 60,
		LIZARD_BEG                 = 61,
		LIZARD_PAIN                = 62,
		LIZARD_DEATH               = 63,
		LIZARD_SPIT                = 64,
		DRONE1_HISSRATTLE          = 65,
		DRONE1_HISSSCREECH         = 66,
		DUKE_TIP2                  = 67,
		FLESH_BURNING              = 68,
		SQUISHED                   = 69,
		TELEPORTER                 = 70,
		ELEVATOR_ON                = 71,
		DUKE_KILLED3               = 72,
		ELEVATOR_OFF               = 73,
		DOOR_OPERATE1              = 74,
		SUBWAY                     = 75,
		SWITCH_ON                  = 76,
		FAN                        = 77,
		DUKE_GETWEAPON3            = 78,
		FLUSH_TOILET               = 79,
		HOVER_CRAFT                = 80,
		EARTHQUAKE                 = 81,
		INTRUDER_ALERT             = 82,
		END_OF_LEVEL_WARN          = 83,
		ENGINE_OPERATING           = 84,
		REACTOR_ON                 = 85,
		COMPUTER_AMBIENCE          = 86,
		GEARS_GRINDING             = 87,
		BUBBLE_AMBIENCE            = 88,
		MACHINE_AMBIENCE           = 89,
		SEWER_AMBIENCE             = 90,
		WIND_AMBIENCE              = 91,
		SOMETHING_DRIPPING         = 92,
		STEAM_HISSING              = 93,
		THEATER_BREATH             = 94,
		BAR_MUSIC                  = 95,
		BOS1_ROAM                  = 96,
		BOS1_RECOG                 = 97,
		BOS1_ATTACK1               = 98,
		BOS1_PAIN                  = 99,
		BOS1_DYING                 =100,
		BOS2_ROAM                  =101,
		BOS2_RECOG                 =102,
		BOS2_ATTACK                =103,
		BOS2_PAIN                  =104,
		BOS2_DYING                 =105,
		GETATOMICHEALTH            =106,
		DUKE_GETWEAPON2            =107,
		BOS3_DYING                 =108,
		SHOTGUN_FIRE               =109,
		PRED_ROAM                  =110,
		PRED_RECOG                 =111,
		PRED_ATTACK                =112,
		PRED_PAIN                  =113,
		PRED_DYING                 =114,
		CAPT_ROAM                  =115,
		CAPT_ATTACK                =116,
		CAPT_RECOG                 =117,
		CAPT_PAIN                  =118,
		CAPT_DYING                 =119,
		PIG_ROAM                   =120,
		PIG_RECOG                  =121,
		PIG_ATTACK                 =122,
		PIG_PAIN                   =123,
		PIG_DYING                  =124,
		RECO_ROAM                  =125,
		RECO_RECOG                 =126,
		RECO_ATTACK                =127,
		RECO_PAIN                  =128,
		RECO_DYING                 =129,
		DRON_ROAM                  =130,
		DRON_RECOG                 =131,
		DRON_ATTACK1               =132,
		DRON_PAIN                  =133,
		DRON_DYING                 =134,
		COMM_ROAM                  =135,
		COMM_RECOG                 =136,
		COMM_ATTACK                =137,
		COMM_PAIN                  =138,
		COMM_DYING                 =139,
		OCTA_ROAM                  =140,
		OCTA_RECOG                 =141,
		OCTA_ATTACK1               =142,
		OCTA_PAIN                  =143,
		OCTA_DYING                 =144,
		TURR_ROAM                  =145,
		TURR_RECOG                 =146,
		TURR_ATTACK                =147,
		DUMPSTER_MOVE              =148,
		SLIM_DYING                 =149,
		BOS3_ROAM                  =150,
		BOS3_RECOG                 =151,
		BOS3_ATTACK1               =152,
		BOS3_PAIN                  =153,
		BOS1_ATTACK2               =154,
		COMM_SPIN                  =155,
		BOS1_WALK                  =156,
		DRON_ATTACK2               =157,
		THUD                       =158,
		OCTA_ATTACK2               =159,
		WIERDSHOT_FLY              =160,
		TURR_PAIN                  =161,
		TURR_DYING                 =162,
		SLIM_ROAM                  =163,
		LADY_SCREAM                =164,
		DOOR_OPERATE2              =165,
		DOOR_OPERATE3              =166,
		DOOR_OPERATE4              =167,
		BORNTOBEWILDSND            =168,
		SHOTGUN_COCK               =169,
		GENERIC_AMBIENCE1          =170,
		GENERIC_AMBIENCE2          =171,
		GENERIC_AMBIENCE3          =172,
		GENERIC_AMBIENCE4          =173,
		GENERIC_AMBIENCE5          =174,
		GENERIC_AMBIENCE6          =175,
		BOS3_ATTACK2               =176,
		GENERIC_AMBIENCE17         =177,
		GENERIC_AMBIENCE18         =178,
		GENERIC_AMBIENCE19         =179,
		GENERIC_AMBIENCE20         =180,
		GENERIC_AMBIENCE21         =181,
		GENERIC_AMBIENCE22         =182,
		SECRETLEVELSND             =183,
		GENERIC_AMBIENCE8          =184,
		GENERIC_AMBIENCE9          =185,
		GENERIC_AMBIENCE10         =186,
		GENERIC_AMBIENCE11         =187,
		GENERIC_AMBIENCE12         =188,
		GENERIC_AMBIENCE13         =189,
		GENERIC_AMBIENCE14         =190,
		GENERIC_AMBIENCE15         =192,
		GENERIC_AMBIENCE16         =193,
		FIRE_CRACKLE               =194,
		BONUS_SPEECH1              =195,
		BONUS_SPEECH2              =196,
		BONUS_SPEECH3              =197,
		PIG_CAPTURE_DUKE           =198,
		BONUS_SPEECH4              =199,
		DUKE_LAND_HURT             =200,
		DUKE_HIT_STRIPPER1         =201,
		DUKE_TIP1                  =202,
		DUKE_KILLED2               =203,
		PRED_ROAM2                 =204,
		PIG_ROAM2                  =205,
		DUKE_GETWEAPON1            =206,
		DUKE_SEARCH2               =207,
		DUKE_CRACK2                =208,
		DUKE_SEARCH                =209,
		DUKE_GET                   =210,
		DUKE_LONGTERM_PAIN         =211,
		MONITOR_ACTIVE             =212,
		NITEVISION_ONOFF           =213,
		DUKE_HIT_STRIPPER2         =214,
		DUKE_CRACK_FIRST           =215,
		DUKE_USEMEDKIT             =216,
		DUKE_TAKEPILLS             =217,
		DUKE_PISSRELIEF            =218,
		SELECT_WEAPON              =219,
		WATER_GURGLE               =220,
		DUKE_GETWEAPON4            =221,
		JIBBED_ACTOR1              =222,
		JIBBED_ACTOR2              =223,
		JIBBED_ACTOR3              =224,
		JIBBED_ACTOR4              =225,
		JIBBED_ACTOR5              =226,
		JIBBED_ACTOR6              =227,
		JIBBED_ACTOR7              =228,
		DUKE_GOTHEALTHATLOW        =229,
		BOSSTALKTODUKE             =230,
		WAR_AMBIENCE1              =231,
		WAR_AMBIENCE2              =232,
		WAR_AMBIENCE3              =233,
		WAR_AMBIENCE4              =234,
		WAR_AMBIENCE5              =235,
		WAR_AMBIENCE6              =236,
		WAR_AMBIENCE7              =237,
		WAR_AMBIENCE8              =238,
		WAR_AMBIENCE9              =239,
		WAR_AMBIENCE10             =240,
		ALIEN_TALK1                =241,
		ALIEN_TALK2                =242,
		EXITMENUSOUND              =243,
		FLY_BY                     =244,
		DUKE_SCREAM                =245,
		SHRINKER_HIT               =246,
		RATTY                      =247,
		INTO_MENU                  =248,
		BONUSMUSIC                 =249,
		DUKE_BOOBY                 =250,
		DUKE_TALKTOBOSSFALL        =251,
		DUKE_LOOKINTOMIRROR        =252,
		PIG_ROAM3                  =253,
		KILLME                     =254,
		DRON_JETSND                =255,
		SPACE_DOOR1                =256,
		SPACE_DOOR2                =257,
		SPACE_DOOR3                =258,
		SPACE_DOOR4                =259,
		SPACE_DOOR5                =260,
		ALIEN_ELEVATOR1            =261,
		VAULT_DOOR                 =262,
		JIBBED_ACTOR13             =263,
		DUKE_GETWEAPON6            =264,
		JIBBED_ACTOR8              =265,
		JIBBED_ACTOR9              =266,
		JIBBED_ACTOR10             =267,
		JIBBED_ACTOR11             =268,
		JIBBED_ACTOR12             =269,
		DUKE_KILLED4               =270,
		DUKE_KILLED5               =271,
		ALIEN_SWITCH1              =272,
		DUKE_STEPONFECES           =273,
		DUKE_LONGTERM_PAIN2        =274,
		DUKE_LONGTERM_PAIN3        =275,
		DUKE_LONGTERM_PAIN4        =276,
		COMPANB2                   =277,
		KTIT                       =278,
		HELICOP_IDLE               =279,
		STEPNIT                    =280,
		SPACE_AMBIENCE1            =281,
		SPACE_AMBIENCE2            =282,
		SLIM_HATCH                 =283,
		RIPHEADNECK                =284,
		FOUNDJONES                 =285,
		ALIEN_DOOR1                =286,
		ALIEN_DOOR2                =287,
		ENDSEQVOL3SND4             =288,
		ENDSEQVOL3SND5             =289,
		ENDSEQVOL3SND6             =290,
		ENDSEQVOL3SND7             =291,
		ENDSEQVOL3SND8             =292,
		ENDSEQVOL3SND9             =293,
		WHIPYOURASS                =294,
		ENDSEQVOL2SND1             =295,
		ENDSEQVOL2SND2             =296,
		ENDSEQVOL2SND3             =297,
		ENDSEQVOL2SND4             =298,
		ENDSEQVOL2SND5             =299,
		ENDSEQVOL2SND6             =300,
		ENDSEQVOL2SND7             =301,
		GENERIC_AMBIENCE23         =302,
		SOMETHINGFROZE             =303,
		DUKE_LONGTERM_PAIN5        =304,
		DUKE_LONGTERM_PAIN6        =305,
		DUKE_LONGTERM_PAIN7        =306,
		DUKE_LONGTERM_PAIN8        =307,
		WIND_REPEAT                =308,
		MYENEMY_ROAM               =309,
		MYENEMY_HURT               =310,
		MYENEMY_DEAD               =311,
		MYENEMY_SHOOT              =312,
		STORE_MUSIC                =313,
		STORE_MUSIC_BROKE          =314,
		ACTOR_GROWING              =315,
		NEWBEAST_ROAM              =316,
		NEWBEAST_RECOG             =317,
		NEWBEAST_ATTACK            =318,
		NEWBEAST_PAIN              =319,
		NEWBEAST_DYING             =320,
		NEWBEAST_SPIT              =321,
		VOL4_1                     =322,
		SUPERMARKET                =323,
		MOUSEANNOY                 =324,
		BOOKEM                     =325,
		SUPERMARKETCRY             =326,
		DESTRUCT                   =327,
		EATFOOD                    =328,
		MAKEMYDAY                  =329,
		WITNESSSTAND               =330,
		VACATIONSPEECH             =331,
		YIPPEE1                    =332,
		YOHOO1                     =333,
		YOHOO2                     =334,
		DOLPHINSND                 =335,
		TOUGHGALSND1               =336,
		TOUGHGALSND2               =337,
		TOUGHGALSND3               =338,
		TOUGHGALSND4               =339,
		TANK_ROAM                  =340,
		BOS4_ROAM                  =341,
		BOS4_RECOG                 =342,
		BOS4_ATTACK                =343,
		BOS4_PAIN                  =344,
		BOS4_DYING                 =345,
		NEWBEAST_ATTACKMISS        =346,
		VOL4_2                     =347,
		COOKINGDEEPFRIER           =348,
		WHINING_DOG                =349,
		DEAD_DOG                   =350,
		LIGHTNING_SLAP             =351,
		THUNDER                    =352,
		HAPPYMOUSESND1             =353,
		HAPPYMOUSESND2             =354,
		HAPPYMOUSESND3             =355,
		HAPPYMOUSESND4             =356,
		ALARM                      =357,
		RAIN                       =358,
		DTAG_GREENRUN              =359,
		DTAG_BROWNRUN              =360,
		DTAG_GREENSCORE            =361,
		DTAG_BROWNSCORE            =362,
		INTRO4_1                   =363,
		INTRO4_2                   =364,
		INTRO4_3                   =365,
		INTRO4_4                   =366,
		INTRO4_5                   =367,
		INTRO4_6                   =368,
		SCREECH                    =369,
		BOSS4_DEADSPEECH           =370,
		BOSS4_FIRSTSEE             =371,
		PARTY_SPEECH               =372,
		POSTAL_SPEECH              =373,
		TGSPEECH                   =374,
		DOGROOMSPEECH              =375,
		SMACKED                    =376,
		MDEVSPEECH                 =377,
		AREA51SPEECH               =378,
		JEEPSOUND                  =379,
		BIGDOORSLAM                =380,
		BOS4_LAY                   =381,
		WAVESOUND                  =382,
		ILLBEBACK                  =383,
		VOL4ENDSND1                =384,
		VOL4ENDSND2                =385,
		EXPANDERHIT                =386,
		SNAKESPEECH                =387,
		EXPANDERSHOOT              =388,
		GETBACKTOWORK              =389,
		JIBBED_ACTOR14             =390,
		JIBBED_ACTOR15             =391,
		INTRO4_B                   =392,
		BIGBANG                    =393,
		SMACKIT                    =394,
		BELLSND                    =395,
		GOAWAY                     =396,
		JOKE                       =397,
		FLAMETHROWER_INTRO         =398,
		FLAMETHROWER_LOOP          =399,
		FLAMETHROWER_END           =400,
		E5L7_DUKE_QUIT_YOU         =401,
	}
}