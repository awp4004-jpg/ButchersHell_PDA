class PDA_Base extends ItemBase
{
    // Server-side only: stores the temporary PDA ID
    string m_PDAID;

    // On/Off state
    protected bool m_IsOn = false;

    void PDA_Base()
    {
        m_PDAID = "";
        m_IsOn = false;
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

    // ==================== Power State ====================
    bool IsOn()
    {
        return m_IsOn;
    }

    void TurnOn()
    {
        if (!m_IsOn)
        {
            m_IsOn = true;

            if (GetCompEM())
            {
                GetCompEM().SwitchOn();
            }

            Print("[PDA] PDA turned ON");
        }
    }

    void TurnOff()
    {
        if (m_IsOn)
        {
            m_IsOn = false;

            if (GetCompEM())
            {
                GetCompEM().SwitchOff();
            }

            Print("[PDA] PDA turned OFF");
        }
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

    override bool CanPutOnBelt(EntityAI parent)
    {
        return true;
    }
}