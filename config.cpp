class CfgPatches
{
	class Butchers_Hell_PDA
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Weapons_Melee",
			"DZ_Gear_Containers",
			"DZ_Gear_Tools",
			"DZ_Weapons_Explosives",
			"DayZExpansion_AI_Scripts"	
		};
	};
};

class CfgMods 
{
	class Butchers_Hell_PDA
	{
		name = "Butchers Hell PDA";
		dir = "ButchersHell_PDA";
		credits = "Butcher,int_x";
		author = "Butcher";
		type = "mod";
		dependencies[] =
		{
			"World"
		};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"ButchersHell_PDA/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"ButchersHell_PDA/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"ButchersHell_PDA/Scripts/5_Mission"};
			};
		};
	};
};