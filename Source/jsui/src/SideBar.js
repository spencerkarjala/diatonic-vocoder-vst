import React from 'react';
import { View } from 'react-juce';
import Dial from './Dial';
import FloorCeilingSlider from './FloorCeilingSlider';
import SVGImage from './SVGImage';
import MidSideMeter from './MidSideMeter';

class SideBar extends React.Component {
  render() {
    return (
      <View
        {...this.props}
        {...styles.root}
      >
        <View {...styles.title}>
          <SVGImage
            source='logo.svg'
            {...styles.titleLogo}
          />
        </View>
        <View {...styles.sliders}>
          <FloorCeilingSlider {...styles.floorCeilingSlider} />
          <MidSideMeter {...styles.midSideMeter} />
        </View>
        <View {...styles.dials}>
          <Dial strokeWidth="5" displayString='Dry' {...styles.dryWetDial} />
          <Dial strokeWidth="5" displayString='Wet' {...styles.dryWetDial} />
        </View>
      </View>
    );
  }
}

const styles = {
  root: {
    flexDirection: 'column',
  },
  title: {
    width: '100%',
    height: '16%',
    alignItems: 'center',
    justifyContent: 'center',
    padding: 8,
  },
  titleLogo: {
    width: '100%',
    height: '100%',
  },
  sliders: {
    width: '100%',
    height: '64%',
  },
  floorCeilingSlider: {
    width: '50%',
    height: '100%',
  },
  midSideMeter: {
    width: '50%',
    height: '100%',
  },
  dials: {
    width: '100%',
    height: '20%',
    alignItems: 'center',
    justifyContent: 'center',
  },
  dryWetDial: {
    minWidth: 60,
    minHeight: 60,
  },
};

export default SideBar;
