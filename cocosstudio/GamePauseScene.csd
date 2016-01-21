<GameFile>
  <PropertyGroup Name="GamePauseScene" Type="Scene" ID="a59fb2d9-613e-467a-99cc-354eef714443" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="120" Speed="1.0000">
        <Timeline ActionTag="-1064615764" Property="Position">
          <PointFrame FrameIndex="0" X="-384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="15" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1064615764" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1064615764" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-470359450" Property="Position">
          <PointFrame FrameIndex="20" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="50" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="55" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="85" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="90" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="120" X="384.0000" Y="512.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-470359450" Property="Scale">
          <ScaleFrame FrameIndex="20" X="2.0000" Y="2.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="50" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="2.0000" Y="2.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="85" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="2.0000" Y="2.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="120" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-470359450" Property="RotationSkew">
          <ScaleFrame FrameIndex="20" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="50" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="85" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="120" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-470359450" Property="FrameEvent">
          <EventFrame FrameIndex="50" Tween="False" Value="countdown3_end" />
          <EventFrame FrameIndex="85" Tween="False" Value="countdown2_end" />
          <EventFrame FrameIndex="120" Tween="False" Value="countdown1_end" />
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="scene_start" StartIndex="0" EndIndex="15">
          <RenderColor A="150" R="255" G="192" B="203" />
        </AnimationInfo>
        <AnimationInfo Name="countdown3" StartIndex="20" EndIndex="50">
          <RenderColor A="150" R="255" G="245" B="238" />
        </AnimationInfo>
        <AnimationInfo Name="countdown2" StartIndex="55" EndIndex="85">
          <RenderColor A="150" R="222" G="184" B="135" />
        </AnimationInfo>
        <AnimationInfo Name="countdown1" StartIndex="90" EndIndex="120">
          <RenderColor A="150" R="245" G="222" B="179" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="155" ctype="GameNodeObjectData">
        <Size X="1536.0000" Y="1024.0000" />
        <Children>
          <AbstractNodeData Name="Panel_GamePause" ActionTag="-1064615764" Tag="165" IconVisible="False" PositionPercentYEnabled="True" RightMargin="768.0000" TopMargin="287.0000" BottomMargin="287.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="191" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="768.0000" Y="450.0000" />
            <Children>
              <AbstractNodeData Name="Text_Title" ActionTag="-1833117736" Tag="166" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="290.0000" RightMargin="290.0000" BottomMargin="359.0000" FontSize="72" LabelText="Pause" OutlineSize="4" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="188.0000" Y="91.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="384.0000" Y="450.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="0.2448" Y="0.2022" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <OutlineColor A="255" R="30" G="144" B="255" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Resume" ActionTag="1608169645" Tag="167" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="170.4000" RightMargin="477.6000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Resume" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="120.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="230.4000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="144" G="238" B="144" />
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
              <AbstractNodeData Name="Button_MainMenu" ActionTag="1868668482" Tag="168" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="477.6000" RightMargin="170.4000" TopMargin="390.0000" BottomMargin="20.0000" TouchEnable="True" FontSize="24" ButtonText="Quit" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
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
              <AbstractNodeData Name="Node_SettingMusic" ActionTag="-797970277" Tag="188" IconVisible="True" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="384.0000" RightMargin="384.0000" TopMargin="225.0000" BottomMargin="225.0000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <Children>
                  <AbstractNodeData Name="Text_Music" ActionTag="131223917" Tag="187" IconVisible="False" LeftMargin="-320.0000" RightMargin="220.0000" TopMargin="-20.0000" BottomMargin="-20.0000" FontSize="40" LabelText="Music" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ShadowEnabled="True" ctype="TextObjectData">
                    <Size X="100.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="-220.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="0" G="0" B="0" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Slider_Music" ActionTag="1689719954" Tag="189" IconVisible="False" LeftMargin="-200.0000" RightMargin="-200.0000" TopMargin="-10.0000" BottomMargin="-10.0000" TouchEnable="True" PercentInfo="50" ctype="SliderObjectData">
                    <Size X="400.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="173" G="216" B="230" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <BackGroundData Type="Default" Path="Default/Slider_Back.png" Plist="" />
                    <ProgressBarData Type="Default" Path="Default/Slider_PressBar.png" Plist="" />
                    <BallNormalData Type="Default" Path="Default/SliderNode_Normal.png" Plist="" />
                    <BallPressedData Type="Default" Path="Default/SliderNode_Press.png" Plist="" />
                    <BallDisabledData Type="Default" Path="Default/SliderNode_Disable.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Button_Mute" ActionTag="-43235090" Tag="190" IconVisible="False" LeftMargin="220.0000" RightMargin="-340.0000" TopMargin="-20.0000" BottomMargin="-20.0000" TouchEnable="True" FontSize="24" ButtonText="Mute" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="120.0000" Y="40.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="220.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="165" B="0" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="384.0000" Y="225.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="384.0000" Y="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.5000" />
            <PreSize X="0.5000" Y="0.4395" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Text_Countdown" Visible="False" ActionTag="-470359450" Tag="179" FrameEvent="countdown3_end" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="368.5000" RightMargin="1136.5000" TopMargin="475.5000" BottomMargin="475.5000" FontSize="60" LabelText="3" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="31.0000" Y="73.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="384.0000" Y="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="165" B="0" />
            <PrePosition X="0.2500" Y="0.5000" />
            <PreSize X="0.0202" Y="0.0713" />
            <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>