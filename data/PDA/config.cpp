class CfgPatches
{
	class Butchers_Hell_Item_PDA
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
            "DZ_Gear_Navigation"
		};
	};
};

class CfgVehicles
{
    class Inventory_Base;
    class PDA_Base: Inventory_Base
    {
        scope = 2;
        displayName = "Personal Digital Assistant";
        descriptionShort = "A rugged personal survival computer.";
		model = "\dz\gear\navigation\GPSReceiver.p3d";
        itemSize[] = {1, 2};
        weight = 250;
        attachments[] = {"BatteryD"};
        rotationFlags = 1;
        energyManager =
        {
            hasIcon = 1;
            autoSwitchOff = 1;
            energyUsagePerSecond = 0.02;
            attachmentAction = 1;
            wetnessExposure = 0.1;
        };
    };
};