import React from 'react';
import { Button } from 'react-bootstrap';

export function PreviewButton ({ slides }) {
  const arrayString = JSON.stringify(slides);
  const previewPresentation = () => {
    window.open(`/preview?data=${encodeURIComponent(arrayString)}`, '_blank');
  }

  return (
    <>
    <Button variant="secondary" onClick={previewPresentation}>Preview</Button>
    </>
  );
}
