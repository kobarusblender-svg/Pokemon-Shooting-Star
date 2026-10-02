Route104_PrettyPetalFlowerShop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route104_PrettyPetalFlowerShop_OnTransition
	.byte 0

Route104_PrettyPetalFlowerShop_OnTransition:
	setflag FLAG_LANDMARK_FLOWER_SHOP
	goto_if_unset FLAG_MET_PRETTY_PETAL_SHOP_OWNER, Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner
	goto_if_unset FLAG_BADGE03_GET, Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner
	setflag FLAG_TEMP_1
	end

Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner::
	setobjectxyperm LOCALID_FLOWER_SHOP_OWNER, 4, 6
	end

Route104_PrettyPetalFlowerShop_EventScript_ShopOwner::
	lock
	faceplayer
	goto_if_set FLAG_TEMP_1, Route104_PrettyPetalFlowerShop_EventScript_Sell
	msgbox Route104_PrettyPetalFlowerShop_Text_ThisIsPrettyPetalFlowerShop, MSGBOX_DEFAULT
	goto_if_set FLAG_MET_PRETTY_PETAL_SHOP_OWNER, Route104_PrettyPetalFlowerShop_EventScript_AlreadyMet
	setflag FLAG_MET_PRETTY_PETAL_SHOP_OWNER
	msgbox Route104_PrettyPetalFlowerShop_Text_IntroLearnAboutBerries, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries
	call_if_eq VAR_RESULT, NO, Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_AlreadyMet::
	msgbox Route104_PrettyPetalFlowerShop_Text_LearnAboutBerries, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries
	call_if_eq VAR_RESULT, NO, Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries::
	msgbox Route104_PrettyPetalFlowerShop_Text_BerriesExplanation, MSGBOX_DEFAULT
	return

Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries::
	msgbox Route104_PrettyPetalFlowerShop_Text_FlowersBringHappiness, MSGBOX_DEFAULT
	return

@ MOD CONTEST selection of shop items
Route104_PrettyPetalFlowerShop_EventScript_Sell::
	message gText_PlayerWhatCanIDoForYou
	waitmessage
	multichoice 0, 0, MULTI_FLOWER_SHOP, FALSE 
	switch VAR_RESULT
	case 0, Route104_PrettyPetalFlowerShop_EventScript_SellFlowers
	case 1, Route104_PrettyPetalFlowerShop_EventScript_SellDecorations
	case 2, Route104_PrettyPetalFlowerShop_EventScript_QuitShopping
	case MULTI_B_PRESSED, Route104_PrettyPetalFlowerShop_EventScript_QuitShopping
	release
	end

@ MOD CONTEST new flower selling dialog
Route104_PrettyPetalFlowerShop_EventScript_SellFlowers::
	msgbox Route104_PrettyPetalFlowerShop_Text_InterestedInFlowers
	waitmessage
	pokemartflower Route104_PrettyPetalFlowerShop_Pokemart_Flowers
	goto Route104_PrettyPetalFlowerShop_EventScript_QuitShopping
	end

Route104_PrettyPetalFlowerShop_EventScript_SellDecorations::
	msgbox Route104_PrettyPetalFlowerShop_Text_InterestedInPlants
	waitmessage
	pokemartdecoration2 Route104_PrettyPetalFlowerShop_Pokemart_Plants
	goto Route104_PrettyPetalFlowerShop_EventScript_QuitShopping
	end
	
Route104_PrettyPetalFlowerShop_EventScript_QuitShopping::
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	waitmessage
	releaseall
	end

	.align 2
Route104_PrettyPetalFlowerShop_Pokemart_Plants:
	.2byte DECOR_RED_PLANT
	.2byte DECOR_TROPICAL_PLANT
	.2byte DECOR_PRETTY_FLOWERS
	.2byte DECOR_COLORFUL_PLANT
	.2byte DECOR_BIG_PLANT
	.2byte DECOR_GORGEOUS_PLANT
	pokemartlistend

@ MOD CONTEST New flower items list
	.align 2
Route104_PrettyPetalFlowerShop_Pokemart_Flowers:
	.2byte ITEM_FLOWER_CHERI
	.2byte ITEM_FLOWER_CHESTO
	.2byte ITEM_FLOWER_PECHA
	.2byte ITEM_FLOWER_RAWST
	.2byte ITEM_FLOWER_ASPEAR
	.2byte ITEM_FLOWER_LEPPA
	.2byte ITEM_FLOWER_ORAN
	.2byte ITEM_FLOWER_PERSIM
	.2byte ITEM_FLOWER_LUM
	.2byte ITEM_FLOWER_SITRUS
	.2byte ITEM_FLOWER_FIGY
	.2byte ITEM_FLOWER_WIKI
	.2byte ITEM_FLOWER_MAGO
	.2byte ITEM_FLOWER_AGUAV
	.2byte ITEM_FLOWER_IAPAPA
	.2byte ITEM_FLOWER_RAZZ
	.2byte ITEM_FLOWER_BLUK
	.2byte ITEM_FLOWER_NANAB
	.2byte ITEM_FLOWER_WEPEAR
	.2byte ITEM_FLOWER_PINAP
	.2byte ITEM_FLOWER_POMEG
	.2byte ITEM_FLOWER_KELPSY
	.2byte ITEM_FLOWER_QUALOT
	.2byte ITEM_FLOWER_HONDEW
	.2byte ITEM_FLOWER_GREPA
	.2byte ITEM_FLOWER_TAMATO
	.2byte ITEM_FLOWER_CORNN
	.2byte ITEM_FLOWER_MAGOST
	.2byte ITEM_FLOWER_RABUTA
	.2byte ITEM_FLOWER_NOMEL
	.2byte ITEM_FLOWER_SPELON
	.2byte ITEM_FLOWER_PAMTRE
	.2byte ITEM_FLOWER_WATMEL
	.2byte ITEM_FLOWER_DURIN
	.2byte ITEM_FLOWER_BELUE
	.2byte ITEM_FLOWER_PAPRIK
	.2byte ITEM_FLOWER_EGGANT
	.2byte ITEM_FLOWER_DRASH
	.2byte ITEM_FLOWER_SKUASH
	.2byte ITEM_FLOWER_PUMKIN
	.2byte ITEM_FLOWER_CHILAN
	.2byte ITEM_FLOWER_MALIUM
	.2byte ITEM_FLOWER_TOPO
	.2byte ITEM_FLOWER_STRIB
	.2byte ITEM_FLOWER_NOION
	.2byte ITEM_FLOWER_NUTPEA
	.2byte ITEM_FLOWER_ROKA
	.2byte ITEM_FLOWER_LIECHI
	.2byte ITEM_FLOWER_GANLON
	.2byte ITEM_FLOWER_SALAC
	.2byte ITEM_FLOWER_PETAYA
	.2byte ITEM_FLOWER_APICOT
	.2byte ITEM_FLOWER_LANSAT
	.2byte ITEM_FLOWER_STARF
	.2byte ITEM_FLOWER_ENIGMA
	pokemartlistend

Route104_PrettyPetalFlowerShop_EventScript_WailmerPailGirl::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_WAILMER_PAIL, Route104_PrettyPetalFlowerShop_EventScript_GiveWailmerPail
	msgbox Route104_PrettyPetalFlowerShop_Text_WailmerPailExplanation, MSGBOX_DEFAULT
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_GiveWailmerPail::
	msgbox Route104_PrettyPetalFlowerShop_Text_YouCanHaveThis, MSGBOX_DEFAULT
	giveitem ITEM_WAILMER_PAIL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox Route104_PrettyPetalFlowerShop_Text_WailmerPailExplanation, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_WAILMER_PAIL
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_RandomBerryGirl::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_FLOWER_SHOP_RECEIVED_BERRY, Route104_PrettyPetalFlowerShop_EventScript_AlreadyReceivedBerry
	msgbox Route104_PrettyPetalFlowerShop_Text_ImGrowingFlowers, MSGBOX_DEFAULT
	random 8
	addvar VAR_RESULT, FIRST_BERRY_INDEX
	giveitem VAR_RESULT
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_FLOWER_SHOP_RECEIVED_BERRY
	msgbox Route104_PrettyPetalFlowerShop_Text_MachineMixesBerries, MSGBOX_DEFAULT
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_AlreadyReceivedBerry::
	msgbox Route104_PrettyPetalFlowerShop_Text_MachineMixesBerries, MSGBOX_DEFAULT
	release
	end
