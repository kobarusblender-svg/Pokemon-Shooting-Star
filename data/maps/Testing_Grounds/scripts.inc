Testing_Grounds_MapScripts::
	.byte 0

Testing_Grounds_EventScript_WAYHereSign::
	msgbox Testing_Grounds_Text_WAYHereSign, MSGBOX_SIGN
	end

	
Testing_Grounds_Text_WAYHereSign:
	.string "This is a debug room...\n"
	.string "Why are you here?$"
	end

	
Testing_Grounds_EventScript_Jaimito::
	trainerbattle_contestsingle TRAINER_JAIMITO, Testing_Grounds_Text_JaimitoIntro, Testing_Grounds_Text_JaimitoDefeated
	msgbox Testing_Grounds_Text_JaimitoPostBattle, MSGBOX_AUTOCLOSE
	end

