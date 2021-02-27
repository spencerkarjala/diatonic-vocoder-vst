import React, { Component } from 'react';
import { View } from 'react-juce';
import Dial from './Dial';

function SVGImage(props) {
  return React.createElement("SVGImage", props, props.children);
}

class Controller extends Component {
  render() {
    return (
      <View {...styles.rootContainer}>
        <View {...styles.patternsContainer}>
          <SVGImage
            source='pattern-editor-background.svg'
            {...styles.backgroundSVG}
          />
        </View>
        <View {...styles.globalsContainer}>
          <SVGImage
            source='global-params-background.svg'
            {...styles.backgroundSVG}
          >
            <View {...styles.globalsBackground}>
              <View {...styles.globalsTop} />
              <View {...styles.globalsBottom}>
                <Dial strokeWidth="5" {...styles.dial} />
                <Dial strokeWidth="5" {...styles.dial} />
                <Dial strokeWidth="5" {...styles.dial} />
              </View>
            </View>
          </SVGImage>
        </View>
      </View>
    );
  }
}

const styles = {
  rootContainer: {
    width: '100%',
    height: '100%',
    padding: 4,
  },
  patternsContainer: {
    width: '40%',
    height: '100%',
    paddingRight: 4,
  },
  globalsContainer: {
    width: '60%',
    height: '100%',
  },
  globalsBackground: {
    height: '100%',
    flex: 1,
    padding: 8,
    flexDirection: 'column',
  },
  backgroundSVG: {
    flex: 1,
    height: '100%',
    flexDirection: 'row',
    alignItems: 'flex-end',
    justifyContent: 'flex-end',
  },
  dial: {
    minWidth: 60,
    minHeight: 60,
    height: '100%',
  },
  fill: {
    flexGrow: 1.0,
  },
  globalsTop: {
    height: '50%',
  },
  globalsBottom: {
    height: '50%',
    justifyContent: 'flex-end',
  },
};

export default Controller;
