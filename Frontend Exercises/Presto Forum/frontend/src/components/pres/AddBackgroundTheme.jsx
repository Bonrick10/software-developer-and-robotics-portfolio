import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';
import { StoreContext } from '../../contexts/StoreContext';
import { AuthContext } from '../../contexts/AuthContext';

export function AddBackgroundTheme ({ presId, activeIndex, setSlides }) {
  const [colour, setColour] = React.useState('');
  const [gradientColourOne, setGradientColourOne] = React.useState('');
  const [gradientColourTwo, setGradientColourTwo] = React.useState('');
  const [gradientDirection, setGradientDirection] = React.useState('');
  const [show, setShow] = React.useState(false);
  const [allSlides, setAllSlides] = React.useState(false);
  const [colourCheck, setColourCheck] = React.useState(false);
  const [gradientCheck, setGradientCheck] = React.useState(false);
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);
  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const handleSwitchChange = (event) => {
    setAllSlides(event.target.checked);
  };

  const handleSwitchColour = (event) => {
    setColourCheck(event.target.checked);
  };

  const handleSwitchGradient = (event) => {
    setGradientCheck(event.target.checked);
  };

  if (colourCheck && gradientCheck) {
    setColourCheck(false);
    setGradientCheck(false);
  }

  if (colour.length === 0) {
    setColour('#ffffff');
  }

  if (gradientDirection.length === 0) {
    console.log('change');
    setGradientDirection('to bottom')
  }
  if (gradientColourOne.length === 0) {
    setGradientColourOne('#ffffff');
  }
  if (gradientColourTwo.length === 0) {
    setGradientColourTwo('#000000');
  }

  const addBackground = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });

      if (allSlides === false) {
        if (colourCheck === true) {
          userStore.store.presentations[presIndex].slides[activeIndex].backgroundColour = colour;
          userStore.store.presentations[presIndex].slides[activeIndex].colour = true;
          userStore.store.presentations[presIndex].slides[activeIndex].gradient = false;
          storeData(userStore);
          setSlides(userStore.store.presentations[presIndex].slides);
          console.log(userStore.store.presentations[presIndex].slides);
          console.log(colourCheck);
          console.log(gradientCheck);
          handleClose();
        } else if (gradientCheck === true) {
          userStore.store.presentations[presIndex].slides[activeIndex].gradientDirection = gradientDirection;
          userStore.store.presentations[presIndex].slides[activeIndex].gradientColourOne = gradientColourOne;
          userStore.store.presentations[presIndex].slides[activeIndex].gradientColourTwo = gradientColourTwo;
          userStore.store.presentations[presIndex].slides[activeIndex].colour = false;
          userStore.store.presentations[presIndex].slides[activeIndex].gradient = true;
          storeData(userStore);
          setSlides(userStore.store.presentations[presIndex].slides);
          console.log(userStore.store.presentations[presIndex].slides[activeIndex]);
          handleClose();
        }
      } else {
        if (colourCheck === true) {
          for (let i = 0; i < userStore.store.presentations[presIndex].slides.length; i++) {
            userStore.store.presentations[presIndex].slides[i].backgroundColour = colour;
            userStore.store.presentations[presIndex].slides[i].colour = true;
            userStore.store.presentations[presIndex].slides[i].gradient = false;
          }
          storeData(userStore);
          setSlides(userStore.store.presentations[presIndex].slides);
          console.log(userStore.store.presentations[presIndex].slides);
          handleClose();
        } else if (gradientCheck === true) {
          for (let i = 0; i < userStore.store.presentations[presIndex].slides.length; i++) {
            userStore.store.presentations[presIndex].slides[i].gradientDirection = gradientDirection;
            userStore.store.presentations[presIndex].slides[i].gradientColourOne = gradientColourOne;
            userStore.store.presentations[presIndex].slides[i].gradientColourTwo = gradientColourTwo;
            userStore.store.presentations[presIndex].slides[i].colour = false;
            userStore.store.presentations[presIndex].slides[i].gradient = true;
          }
          storeData(userStore);
          setSlides(userStore.store.presentations[presIndex].slides);
          console.log(userStore.store.presentations[presIndex].slides);
          handleClose();
        }
      }
    } catch (err) {
      alert(err.response);
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      addBackground();
    }
  }
  return (
    <>
    <Button variant="info" onClick={handleShow}>Add Slide Background</Button>
    <Modal
      show={show}
      onHide={handleClose}
      backdrop="static"
      keyboard={false}
    >
      <Modal.Header closeButton>
        <Modal.Title>Slide Background</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        <Form>
        <Form.Group className="mb-3">
            <Form.Check
              type="switch"
              id="custom-switch"
              label="Colour"
              checked={colourCheck}
              onChange={handleSwitchColour}
            />
            {colourCheck && (
              <>
              <Form.Label className="bootStrap" htmlFor="exampleColorInput">Pick a colour</Form.Label>
              <Form.Control className="bootStrap"
              type="color"
              id="exampleColorInput"
              defaultValue="#ffffff"
              title="Choose your color"
              onChange={e => setColour(e.target.value)}
              autoFocus
              />
              </>
            )}
            <Form.Check
              type="switch"
              id="custom-switch"
              label="Gradient"
              checked={gradientCheck}
              onChange={handleSwitchGradient}
            />
            {gradientCheck && (
              <>
              <Form.Control className="bootStrap"
              type="gradient-direction"
              placeholder="Gradient Direction (default: to bottom)"
              onChange={e => setGradientDirection(e.target.value)}
              autoFocus
              />
              <Form.Label className="bootStrap" htmlFor="exampleColorInput">Colour one</Form.Label>
              <Form.Control className="bootStrap"
                type="color"
                id="exampleColorInput"
                defaultValue="#ffffff"
                onChange={e => setGradientColourOne(e.target.value)}
                title="Choose your color"
              />
              <Form.Label className="bootStrap" htmlFor="exampleColorInput">Colour two</Form.Label>
              <Form.Control className="bootStrap"
                type="color"
                id="exampleColorInput"
                defaultValue="#000000"
                onChange={e => setGradientColourTwo(e.target.value)}
                title="Choose your color"
              />
              </>
            )}
            <Form.Check
              type="switch"
              id="custom-switch"
              label="Apply to all slides"
              checked={allSlides}
              onChange={handleSwitchChange}
            />
          </Form.Group>
        </Form>
      </Modal.Body>
      <Modal.Footer>
        <Button variant="danger" onClick={handleClose}>
          Cancel
        </Button>
        <Button variant="primary" onKeyDown={enterKeyDown} onClick={addBackground}>Create</Button>
      </Modal.Footer>
    </Modal>
  </>
  );
}
