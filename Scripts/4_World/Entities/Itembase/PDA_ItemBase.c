class PDA_Base extends ItemBase
{
    // Server-side only: stores the temporary PDA ID
    string m_PDAID;

    void PDA_Base()
    {
        m_PDAID = "";
    }

    // ==================== PDA ID ====================
    void GeneratePDAID()
    {
        if (m_PDAID == "")
        {
            m_PDAID = (Math.RandomInt(100000, 999999)).ToString();
            Print("[PDA] New PDA ID generated: " + m_PDAID);
        }
    }

    string GetPDAID()
    {
        return m_PDAID;
    }

    // ==================== Power State (using native functions) ====================
    bool IsPoweredOn()
    {
        return IsOn(); // Uses the native IsOn() from InventoryItem
    }

    void TurnOn()
    {
        SwitchOn(true); // Native function - starts battery consumption
        Print("[PDA] PDA turned ON");
    }

    void TurnOff()
    {
        SwitchOn(false); // Native function - stops battery consumption
        Print("[PDA] PDA turned OFF");
    }

    // ==================== Actions ====================
    override void SetActions()
    {
        super.SetActions();

        AddAction(ActionTurnOnPDA);
        AddAction(ActionTurnOffPDA);
        AddAction(ActionOpenPDA);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return true;
    }
}