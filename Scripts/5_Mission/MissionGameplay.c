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