<GameFile>
  <PropertyGroup Name="GamePauseScene" Type="Scene" ID="a59fb2d9-613e-467a-99cc-354eef714443" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="120" Speed="1.0000">
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
        <AnimationInfo Name="countdown3" StartIndex="20" EndIndex="50">
          <RenderColor A="255" R="255" G="245" B="238" />
        </AnimationInfo>
        <AnimationInfo Name="countdown2" StartIndex="55" EndIndex="85">
          <RenderColor A="255" R="222" G="184" B="135" />
        </AnimationInfo>
        <AnimationInfo Name="countdown1" StartIndex="90" EndIndex="120">
          <RenderColor A="255" R="245" G="222" B="179" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="155" ctype="GameNodeObjectData">
        <Size X="1536.0000" Y="1024.0000" />
        <Children>
          <AbstractNodeData Name="Panel_GamePause" ActionTag="-1064615764" Tag="165" IconVisible="False" PositionPercentYEnabled="True" RightMargin="768.0000" TopMargin="387.0000" BottomMargin="387.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="191" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="768.0000" Y="250.0000" />
            <Children>
              <AbstractNodeData Name="Text_Title" ActionTag="-1833117736" Tag="166" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="290.0000" RightMargin="290.0000" BottomMargin="159.0000" FontSize="72" LabelText="Pause" OutlineSize="4" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="188.0000" Y="91.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="384.0000" Y="250.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="0.2448" Y="0.3640" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <OutlineColor A="255" R="30" G="144" B="255" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_Resume" ActionTag="1608169645" Tag="167" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="150.4000" RightMargin="457.6000" TopMargin="140.0000" BottomMargin="50.0000" TouchEnable="True" FontSize="32" ButtonText="Resume" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="160.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="230.4000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="144" G="238" B="144" />
                <PrePosition X="0.3000" Y="0.2000" />
                <PreSize X="0.2083" Y="0.2400" />
                <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_MainMenu" ActionTag="1868668482" Tag="168" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="457.6000" RightMargin="150.4000" TopMargin="140.0000" BottomMargin="50.0000" TouchEnable="True" FontSize="32" ButtonText="Quit" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="160.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="537.6000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="173" G="216" B="230" />
                <PrePosition X="0.7000" Y="0.2000" />
                <PreSize X="0.2083" Y="0.2400" />
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
            <Position X="384.0000" Y="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.5000" />
            <PreSize X="0.5000" Y="0.2441" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Text_Countdown" Visible="False" ActionTag="-470359450" Tag="179" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="368.5000" RightMargin="1136.5000" TopMargin="475.5000" BottomMargin="475.5000" FontSize="60" LabelText="3" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="31.0000" Y="73.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="384.0000" Y="512.0000" />
            <Scale ScaleX="2.0000" ScaleY="2.0000" />
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