<GameFile>
  <PropertyGroup Name="MainMenuScene" Type="Scene" ID="a2ee0952-26b5-49ae-8bf9-4f1d6279b798" Version="2.3.3.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="-868953858" Property="Position">
          <PointFrame FrameIndex="0" X="10.0000" Y="1014.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="10.0000" Y="1070.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="10.0000" Y="1014.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-868953858" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-868953858" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-868953858" Property="FrameEvent">
          <EventFrame FrameIndex="60" Tween="False" Value="Scene_Start_End" />
        </Timeline>
      </Animation>
      <ObjectData Name="Scene" ctype="GameNodeObjectData">
        <Size X="1536.0000" Y="1024.0000" />
        <Children>
          <AbstractNodeData Name="Text_Title" ActionTag="-858646474" Tag="86" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="81.5000" RightMargin="849.5000" TopMargin="51.2000" BottomMargin="849.8000" FontSize="100" LabelText="Endless Jump" OutlineSize="4" OutlineEnabled="True" ShadowOffsetX="4.0000" ShadowOffsetY="-4.0000" ShadowEnabled="True" ctype="TextObjectData">
            <Size X="605.0000" Y="123.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="384.0000" Y="972.8000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.9500" />
            <PreSize X="0.3939" Y="0.1201" />
            <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
            <OutlineColor A="255" R="30" G="144" B="255" />
            <ShadowColor A="255" R="0" G="0" B="0" />
          </AbstractNodeData>
          <AbstractNodeData Name="Text_TotalDead" ActionTag="-184426614" Tag="100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="210.5000" RightMargin="978.5000" TopMargin="204.8000" BottomMargin="764.2000" FontSize="48" LabelText="Total Dead: 999" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="347.0000" Y="57.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="384.0000" Y="819.2000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.8000" />
            <PreSize X="0.2259" Y="0.0537" />
            <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
            <OutlineColor A="255" R="0" G="0" B="0" />
            <ShadowColor A="255" R="0" G="0" B="0" />
          </AbstractNodeData>
          <AbstractNodeData Name="Button_LevelEditor" ActionTag="-1543634144" Tag="85" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="1072.0000" RightMargin="304.0000" TopMargin="840.4000" BottomMargin="123.6000" TouchEnable="True" FontSize="24" ButtonText="Level Editor" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="160.0000" Y="60.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="1152.0000" Y="153.6000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="165" B="0" />
            <PrePosition X="0.7500" Y="0.1500" />
            <PreSize X="0.1042" Y="0.0586" />
            <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="ScrollView_Levels" ActionTag="-1886580383" Tag="110" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="134.0000" RightMargin="902.0000" TopMargin="314.4000" BottomMargin="109.6000" TouchEnable="True" ClipAble="True" BackColorAlpha="51" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
            <Size X="500.0000" Y="600.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="384.0000" Y="409.6000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.4000" />
            <PreSize X="0.3255" Y="0.5859" />
            <SingleColor A="255" R="229" G="229" B="229" />
            <FirstColor A="255" R="191" G="191" B="191" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
            <InnerNodeSize Width="500" Height="600" />
          </AbstractNodeData>
          <AbstractNodeData Name="Button_Setting" ActionTag="-868953858" Tag="186" IconVisible="False" LeftMargin="10.0000" RightMargin="1406.0000" TopMargin="10.0000" BottomMargin="974.0000" TouchEnable="True" FontSize="24" ButtonText="Setting" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="120.0000" Y="40.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="10.0000" Y="1014.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="173" G="216" B="230" />
            <PrePosition X="0.0065" Y="0.9902" />
            <PreSize X="0.0781" Y="0.0391" />
            <FontResource Type="Normal" Path="Resource/fzzj.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>