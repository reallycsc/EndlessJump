<GameFile>
  <PropertyGroup Name="GameOverScene" Type="Scene" ID="49acf727-9285-4562-b1d3-b1fc9bec9e6a" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="-1961623313" Property="Position">
          <PointFrame FrameIndex="0" X="384.0000" Y="300.0000">
            <EasingData Type="29" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="384.0000" Y="280.0002">
            <EasingData Type="29" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1961623313" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.5000" Y="0.5000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1961623313" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.0000" Y="0.0000">
            <EasingData Type="29" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1587651597" Property="Position">
          <PointFrame FrameIndex="0" X="-384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="15" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="1587651597" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1587651597" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Scene" Tag="155" ctype="GameNodeObjectData">
        <Size X="1536.0000" Y="1024.0000" />
        <Children>
          <AbstractNodeData Name="Panel_GameOver" ActionTag="1587651597" Tag="169" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-768.0000" RightMargin="1536.0000" TopMargin="287.0000" BottomMargin="287.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="191" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="768.0000" Y="450.0000" />
            <Children>
              <AbstractNodeData Name="Text_Title" ActionTag="-836941380" Tag="170" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="188.5000" RightMargin="188.5000" BottomMargin="359.0000" FontSize="72" LabelText="Level Name" OutlineSize="4" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="391.0000" Y="91.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="384.0000" Y="450.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="0.5091" Y="0.2022" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <OutlineColor A="255" R="30" G="144" B="255" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Node_EnlargeAnimation" ActionTag="-1961623313" Tag="171" IconVisible="True" LeftMargin="384.0000" RightMargin="384.0000" TopMargin="150.0000" BottomMargin="300.0000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <Children>
                  <AbstractNodeData Name="Text_DeadNumber" ActionTag="-1281327057" Tag="172" IconVisible="False" LeftMargin="-142.5000" RightMargin="-142.5000" TopMargin="-51.0000" BottomMargin="10.0000" FontSize="36" LabelText="0 deads this level." HorizontalAlignmentType="HT_Center" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="285.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position Y="10.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_DeadNumberAll" ActionTag="1361296020" Tag="173" IconVisible="False" LeftMargin="-108.0000" RightMargin="-108.0000" TopMargin="-11.0000" BottomMargin="-30.0000" FontSize="36" LabelText="0 deads total." HorizontalAlignmentType="HT_Center" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="216.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position Y="-30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="384.0000" Y="300.0000" />
                <Scale ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6667" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Next" ActionTag="1633137155" Tag="174" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="324.0000" RightMargin="324.0000" TopMargin="300.0000" BottomMargin="110.0000" TouchEnable="True" FontSize="24" ButtonText="Next" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="384.0000" Y="110.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="165" B="0" />
                <PrePosition X="0.5000" Y="0.2444" />
                <PreSize X="0.1563" Y="0.0889" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_UnlockNow" ActionTag="2066523340" Tag="147" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="324.0000" RightMargin="324.0000" TopMargin="300.0000" BottomMargin="110.0000" TouchEnable="True" FontSize="20" ButtonText="Unlock Now" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="384.0000" Y="110.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="0" />
                <PrePosition X="0.5000" Y="0.2444" />
                <PreSize X="0.1563" Y="0.0889" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_NextRequirement" ActionTag="-1015811756" Tag="175" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="290.0000" RightMargin="290.0000" TopMargin="342.0000" BottomMargin="80.0000" FontSize="24" LabelText="(Total dead &lt; 10)" OutlineSize="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                <Size X="188.0000" Y="28.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="384.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1778" />
                <PreSize X="0.2448" Y="0.0622" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Retry" ActionTag="266935803" Tag="176" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="170.4000" RightMargin="477.6000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Retry" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="230.4000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="192" B="203" />
                <PrePosition X="0.3000" Y="0.0444" />
                <PreSize X="0.1563" Y="0.0889" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_MainMenu" ActionTag="-97012319" Tag="177" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="477.6000" RightMargin="170.4000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Quit" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="537.6000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="173" G="216" B="230" />
                <PrePosition X="0.7000" Y="0.0444" />
                <PreSize X="0.1563" Y="0.0889" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-384.0000" Y="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="-0.2500" Y="0.5000" />
            <PreSize X="0.5000" Y="0.4395" />
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