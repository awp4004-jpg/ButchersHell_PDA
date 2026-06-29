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

        // Reset PDA login session if "Stay Logged In" is disabled
        if (!PDA_Menu.s_IsLoggedInThisSession)
        {
            // Already logged out
            Print("[PDA] Player disconnected - session was already ended");
        }
        else
        {
            // Check if user wanted to stay logged in
            string stayLoggedIn;
            if (GetGame().GetProfileString("PDA_StayLoggedIn", stayLoggedIn))
            {
                if (stayLoggedIn != "true")
                {
                    PDA_Menu.s_IsLoggedInThisSession = false;
                    Print("[PDA] Player disconnected - Stay Logged In was OFF → session reset");
                }
                else
                {
                    Print("[PDA] Player disconnected - Stay Logged In was ON → keeping session");
                }
            }
            else
            {
                // Default behavior if no setting saved
                PDA_Menu.s_IsLoggedInThisSession = false;
                Print("[PDA] Player disconnected - no Stay Logged In setting found → session reset");
            }
        }
    }
}