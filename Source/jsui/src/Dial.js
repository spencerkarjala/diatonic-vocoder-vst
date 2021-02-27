import React from 'react';
import { View, Text } from 'react-juce';

function _Dial(props) {
  return React.createElement("DialComponent", props, props.children);
}

class Dial extends React.Component {
  constructor(props) {
    super(props);

    this.handleMeasure = this.handleMeasure.bind(this);
    this.handleMouseDown = this.handleMouseDown.bind(this);
    this.handleMouseUp = this.handleMouseUp.bind(this);
    this.handleMouseDrag = this.handleMouseDrag.bind(this);

    const initialState = {
      value: 0.5,
      color: "#00b894",
      yDragPosition: -1,
      width: 0,
      height: 0,
      displayString: props.displayString || '',
    }
    this.state = initialState;
  }

  handleMeasure(event) {
    this.setState({
      ...this.state,
      width: event.width,
      height: event.width,
    })
  }

  handleMouseDown(event) {
    this.setState({
      ...this.state,
      yDragPosition: event.y,
    });
  }

  handleMouseUp() {
    this.setState({
      ...this.state,
      yDragPosition: -1
    })
  }

  handleMouseDrag(event) {
    const { value, height, yDragPosition } = this.state;
    const delta = yDragPosition - event.y;

    this.setState({
      ...this.state,
      value: Math.max(Math.min(value + (delta / height), 1.0), 0.0),
      yDragPosition: event.y,
    });
  }

  render() {
    const { width, height, value, displayString } = this.state;

    return (
      <View
        {...this.props}
        onMeasure={this.handleMeasure}
        {...styles.root}
      >
        <Text
          height='25%'
          fontFamily='Franklin Gothic Medium'
          {...styles.label}
        >
          {displayString}
        </Text>
          <_Dial
            value={value}
            width={width}
            height={height*0.75 - 8}
            arcWidth={width * 0.08}
            arcLength={0.8}
            onMouseDown={this.handleMouseDown}
            onMouseUp={this.handleMouseUp}
            onMouseDrag={this.handleMouseDrag}
            {...styles.dial}
          />
      </View>
    );
  }
}

const styles = {
  root: {
    flexDirection: 'column',
    alignItems: 'center',
    justifyContent: 'center'
  },
  dial: {
    marginTop: 8,
    minHeight: 20,
    minWidth: 20,
  },
  label: {
    color: 'white',
    fontSize: 16.0,
  },
}

export default Dial;
