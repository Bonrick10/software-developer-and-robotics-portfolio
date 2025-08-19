import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function EditTextBox ({ activeIndex, setSlides, presId, textTarget, textIndex, editText, setEditText }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [text, setText] = React.useState(textTarget.text);
  const [width, setWidth] = React.useState(textTarget.width);
  const [height, setHeight] = React.useState(textTarget.height);
  const [fontSize, setFontSize] = React.useState(textTarget.fontSize);
  const [textColour, setTextColour] = React.useState(textTarget.textColour);
  const [fontFamily, setFontFamily] = React.useState(textTarget.fontFamily);

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const EditText = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];
      const currText = currSlide.text[textIndex];

      currText.width = width;
      currText.height = height;
      currText.text = text;
      currText.fontSize = fontSize;
      currText.textColour = textColour;
      currText.fontFamily = fontFamily;

      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
      setEditText(false);
    } catch (err) {
      alert(err.response);
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      EditText();
    }
  }

  return (
    <>
    {editText && (<Button variant="secondary" onClick={handleShow}>Edit Text</Button>)}
    <Modal
      show={show}
      onHide={handleClose}
      backdrop="static"
      keyboard={false}
    >
      <Modal.Header closeButton>
        <Modal.Title>Edit Text</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        <Form>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                defaultValue={textTarget.width}
                min="1"
                max="100"
                step=".1"
                placeholder="New width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Height</Form.Label>
              <Form.Control
                type="number"
                defaultValue={textTarget.height}
                min="1"
                max="100"
                step=".1"
                placeholder="New height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
            <Form.Label>Text</Form.Label>
            <Form.Control
              type="text"
              defaultValue={textTarget.text}
              placeholder="New text"
              onChange={e => setText(e.target.value)}
              autoFocus
            />
          </Form.Group>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
            <Form.Label>Font Size</Form.Label>
            <Form.Control
              type="number"
              defaultValue={textTarget.fontSize}
              placeholder="New font size (em)"
              onChange={e => setFontSize(e.target.value)}
              autoFocus
            />
          </Form.Group>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
            <Form.Label>Text Colour</Form.Label>
            <Form.Control
              type="text-colour"
              defaultValue={textTarget.textColour}
              placeholder="#000000"
              onChange={e => setTextColour(e.target.value)}
              autoFocus
            />
          </Form.Group>
          <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Font (optional)</Form.Label>
              <Form.Control
                type="text"
                defaultValue={textTarget.fontFamily}
                placeholder="Verdana"
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
        <Button variant="primary" onKeyDown={enterKeyDown} onClick={EditText}>Create</Button>
      </Modal.Footer>
    </Modal>
    </>
  );
}
