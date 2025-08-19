import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function AddTextModal ({ activeIndex, setSlides, presId }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [text, setText] = React.useState('');
  const [width, setWidth] = React.useState(0);
  const [height, setHeight] = React.useState(0);
  const [fontSize, setFontSize] = React.useState(0);
  const [textColour, setTextColour] = React.useState(0);
  const [fontFamily, setFontFamily] = React.useState('');

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const addText = async () => {
    if (text.length === 0) {
      alert('Please enter text');
    }
    if (isNaN(width) || isNaN(height) || isNaN(fontSize)) {
      alert('Please enter a number for width, height or font size')
    }
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];

      const newText = {
        width: width,
        height: height,
        text: text,
        top: 0,
        left: 0,
        fontSize: fontSize,
        textColour: textColour,
        fontFamily: fontFamily,
        zIndex: currSlide.zIndex
      };
      currSlide.zIndex++;
      currSlide.text.push(newText);
      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
    } catch (err) {
      alert(err.response);
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      addText();
    }
  }

  return (
    <>
    <Button variant="info" onClick={handleShow}>Add Text</Button>
    <Modal
        show={show}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>TEXT</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Size</Form.Label>
              <Form.Control
                type="width"
                placeholder="width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
              <Form.Control
                type="height"
                placeholder="height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Text</Form.Label>
              <Form.Control
                type="text"
                placeholder="Text"
                onChange={e => setText(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Font Size</Form.Label>
              <Form.Control
                type="font-size"
                placeholder="1(em)"
                onChange={e => setFontSize(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Text Colour (optional)</Form.Label>
              <Form.Control
                type="text-colour"
                placeholder="#000000"
                onChange={e => setTextColour(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Font (optional)</Form.Label>
              <Form.Control
                type="font-family"
                placeholder="Arial"
                onChange={e => setFontFamily(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onKeyDown={enterKeyDown} onClick={addText}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
