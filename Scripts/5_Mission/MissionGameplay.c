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

        // Force logout from PDA every time the player leaves the server
        // (disconnect, crash, quit, etc.)
        PDA_Menu.s_IsLoggedInThisSession = false;
        PDA_Menu.s_PinEnabled = false;           // optional reset

        // Clear saved username so they have to fully log in again next time
        GetGame().SetProfileString("PDA_CurrentUsername", "");

        Print("[PDA] Player left server → PDA session forcefully reset (force logout on reconnect)");
    }
}