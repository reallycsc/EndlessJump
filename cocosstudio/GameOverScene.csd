<GameFile>
  <PropertyGroup Name="GameOverScene" Type="Scene" ID="49acf727-9285-4562-b1d3-b1fc9bec9e6a" Version="2.3.3.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="-1437730575" Property="Position">
          <PointFrame FrameIndex="0" X="300.0000" Y="300.0000">
            <EasingData Type="29" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="300.0000" Y="280.0000">
            <EasingData Type="29" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1437730575" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.5000" Y="0.5000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1437730575" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Scene" Tag="155" ctype="GameNodeObjectData">
        <Size X="1536.0000" Y="1024.0000" />
        <Children>
          <AbstractNodeData Name="Panel_GameOver" ActionTag="79196254" Tag="156" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="84.0000" RightMargin="852.0000" TopMargin="287.0000" BottomMargin="287.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="191" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="600.0000" Y="450.0000" />
            <Children>
              <AbstractNodeData Name="Text_Title" ActionTag="-1576857839" Tag="157" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="120.0000" RightMargin="120.0000" BottomMargin="378.0000" FontSize="72" LabelText="Level Name" OutlineSize="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                <Size X="360.0000" Y="72.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="300.0000" Y="450.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="0.6000" Y="0.1600" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Node_EnlargeAnimation" ActionTag="-1437730575" Tag="158" IconVisible="True" PositionPercentXEnabled="True" LeftMargin="300.0000" RightMargin="300.0000" TopMargin="170.0000" BottomMargin="280.0000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <Children>
                  <AbstractNodeData Name="Text_DeadNumber" ActionTag="2106556479" Tag="159" IconVisible="False" LeftMargin="-171.0000" RightMargin="-171.0000" TopMargin="-46.0000" BottomMargin="10.0000" FontSize="36" LabelText="0 deads this level." HorizontalAlignmentType="HT_Center" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="342.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position Y="10.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_DeadNumberAll" ActionTag="1330626684" Tag="115" IconVisible="False" LeftMargin="-126.0000" RightMargin="-126.0000" TopMargin="-6.0000" BottomMargin="-30.0000" FontSize="36" LabelText="0 deads total." HorizontalAlignmentType="HT_Center" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="252.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position Y="-30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="300.0000" Y="280.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6222" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Next" ActionTag="-1267946466" Tag="160" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="240.0000" RightMargin="240.0000" TopMargin="300.0000" BottomMargin="110.0000" TouchEnable="True" FontSize="24" ButtonText="Next" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="300.0000" Y="110.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="165" B="0" />
                <PrePosition X="0.5000" Y="0.2444" />
                <PreSize X="0.2000" Y="0.0889" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_NextRequirement" ActionTag="498048940" Tag="161" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="198.0000" RightMargin="198.0000" TopMargin="346.0000" BottomMargin="80.0000" FontSize="24" LabelText="(Total dead &lt; 10)" OutlineSize="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                <Size X="204.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="300.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1778" />
                <PreSize X="0.3400" Y="0.0533" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Retry" ActionTag="-1899338697" Tag="162" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="120.0000" RightMargin="360.0000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Retry" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="180.0000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="144" G="238" B="144" />
                <PrePosition X="0.3000" Y="0.0444" />
                <PreSize X="0.2000" Y="0.0889" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_MainMenu" ActionTag="-1074769046" Tag="163" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="360.0000" RightMargin="120.0000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Quit" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="420.0000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="173" G="216" B="230" />
                <PrePosition X="0.7000" Y="0.0444" />
                <PreSize X="0.2000" Y="0.0889" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_Finish" ActionTag="-1351601877" Tag="100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="220.0000" BottomMargin="130.0000" IsCustomSize="True" FontSize="22" LabelText="Thank you for playing!&#xA;New levels will coming soon in next big update.&#xA;Now you can try to get the ultimate achievement." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                <Size X="550.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="180.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="165" B="0" />
                <PrePosition X="0.5000" Y="0.4000" />
                <PreSize X="0.9167" Y="0.2222" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="384.0000" Y="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.5000" />
            <PreSize X="0.3906" Y="0.4395" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>