import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function EditCodeBox ({ activeIndex, setSlides, presId, codeTarget, codeIndex, editCode, setEditCode }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [width, setWidth] = React.useState(codeTarget.width);
  const [height, setHeight] = React.useState(codeTarget.height);
  const [code, setCode] = React.useState(codeTarget.code);
  const [lang, setLang] = React.useState(codeTarget.lang);
  const [fontSize, setFontSize] = React.useState(codeTarget.fontSize);

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const guessLang = () => {
    // TODO: Probably add some patterns to improve guessing
    const langPatterns = {
      c: [
        /#(include.*?>|define.*)/g, // cPreprocessor
        /\b(typedef.*?;|struct.*?(?=\s*?\{))/g, // cStruct
        /\b\w+\s*->\s*\w+\b/g, // cPointers
        /[a-zA-Z]+\s+\*\s+([a-zA-Z_]\w*)\s*\([a-zA-Z]*\s*([a-zA-Z_]\w*)\)\s*\{/g, // cFunctionPointerReturn
        /[a-zA-Z]+\s*\*?\s*([a-zA-Z_]\w*)\(.*[a-zA-Z]+\s*\*\s*([a-zA-Z_]\w*).*\)\s*\{/g // cFunctionPointerArg
      ],
      javascript: [
        /import\s+\*\s+as\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s+from\s+'([^']+)'/, // jsImport
        /class\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*(?:extends\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*)?{/, // jsClassDeclaration
        /function\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*\(([^)]*)\)\s*{/, // jsFunctionDeclaration
        /([a-zA-Z_$][0-9a-zA-Z_$]*)\s*\(([^)]*)\)/, // jsFunctionCall
        /var\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsVarDeclaration
        /let\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsLetDeclaration
        /const\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsConstDeclaration
      ],
      python: [
        /(class|def|if|while|else|for).*:/g, // pythonKeywords
        /\[((\d+|("|').+\3),)*(\d+|("|').+\5)\]/g, // pythonLists
        /\bprint(\s*("|').+\2|\(.*\))/g, // pythonPrint
      ],
    }
    const newLang = Object.keys(langPatterns).find(lang =>
      langPatterns[lang].some(pattern =>
        pattern.test(code)));
    setLang(newLang);
  }

  const editCodeObject = async () => {
    if (!width || !height || !code || !fontSize) {
      alert('Please fill empty fields');
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
      console.log(codeIndex);
      const currCode = currSlide.code[codeIndex];

      guessLang();
      currCode.width = width;
      currCode.height = height;
      currCode.code = code;
      currCode.lang = lang;
      currCode.fontSize = fontSize;

      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
      setEditCode(false);
    } catch (err) {
      alert(err.response);
    }
  }

  return (
    <>
    {editCode && (<Button variant="secondary" onClick={handleShow}>Edit Code</Button>)}
    <Modal
        show={show}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>Edit Code Block</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                defaultValue={codeTarget.width}
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
                defaultValue={codeTarget.height}
                min="1"
                max="100"
                step=".1"
                placeholder="New height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Code Block</Form.Label>
              <Form.Control
                type="textArea"
                defaultValue={codeTarget.code}
                placeholder="Code in JavaScript, C or Python"
                onChange={e => setCode(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Font Size</Form.Label>
              <Form.Control
                type="number"
                defaultValue={codeTarget.fontSize}
                min="0.01"
                step=".01"
                placeholder="Font size in em"
                onChange={e => setFontSize(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onClick={editCodeObject}>Confirm Edits</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
