modded class MissionBase
{
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);

        if (!menu)
        {
            if (id == PDA_MENU.PDA)
            {
                menu = new PDA_Menu();
            }

            if (menu)
            {
                menu.SetID(id);
            }
        }
        return menu;
    }
}
modded class MissionGameplay
{
    override void OnMissionFinish()
    {
        super.OnMissionFinish();

        // Reset PDA login state when disconnecting from server
        PDA_Menu.s_IsLoggedInThisSession = false;

        Print("[PDA] Login session reset (disconnected from server)");
    }
}